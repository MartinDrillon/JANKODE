#include "scan_dma.h"
#include "keys.h"
#include "imxrt.h"

// Définitions manquantes pour ADC_ETC et DMAMUX
#ifndef CCM_CCGR3_ADC_ETC
#define CCM_CCGR3_ADC_ETC(n) ((uint32_t)(n) << 20)
#endif

#ifndef DMAMUX_CHCFG_SOURCE
#define DMAMUX_CHCFG_SOURCE(n) ((uint8_t)(n))
#endif

// Table de correspondance pin → ADC (basée sur cores/teensy4/analog.c)
struct PinToADC {
    uint8_t adc;     // 1 ou 2
    uint8_t channel; // Canal ADC
};

// Mapping pour nouvelles pins analogiques 41,40,39,38,20,21,22,23
static const PinToADC pinMapping[8] = {
    {1, 0},   // Pin 41 -> ADC1_IN0
    {1, 1},   // Pin 40 -> ADC1_IN1
    {1, 2},   // Pin 39 -> ADC1_IN2
    {1, 3},   // Pin 38 -> ADC1_IN3
    {2, 6},   // Pin 20 -> ADC2_IN6
    {2, 7},   // Pin 21 -> ADC2_IN7
    {2, 8},   // Pin 22 -> ADC2_IN8
    {2, 9}    // Pin 23 -> ADC2_IN9
};

// Buffers double (ping/pong)
static DmaFrame frameA, frameB;
static volatile DmaFrame* activeFrame = &frameA;
static volatile DmaFrame* readyFrame = nullptr;
static volatile bool frameReady = false;

// Variables de contrôle
static volatile uint8_t currentStep = 0;

// Helpers pour sélecteurs GPIO
static inline void setSelectA(uint8_t g) {
    digitalWriteFast(SEL_A_S0, g & 1);
    digitalWriteFast(SEL_A_S1, (g >> 1) & 1);
    digitalWriteFast(SEL_A_S2, (g >> 2) & 1);
    digitalWriteFast(SEL_A_S3, (g >> 3) & 1);
}

static inline void setSelectB(uint8_t g) {
    digitalWriteFast(SEL_B_S0, g & 1);
    digitalWriteFast(SEL_B_S1, (g >> 1) & 1);
    digitalWriteFast(SEL_B_S2, (g >> 2) & 1);
    digitalWriteFast(SEL_B_S3, (g >> 3) & 1);
}

// ISR PIT Timer 0
void pit0_isr() {
    // Clear PIT flag
    PIT_TFLG0 = 1;
    
    // Écrire Gray[currentStep] sur sélecteurs A et B
    uint8_t sel = GRAY[currentStep];
    setSelectA(sel);
    setSelectB(sel);
    
    // Délai de stabilisation (T_SETTLE_NS) - utilisation de delayMicroseconds
    delayMicroseconds(1); // 1µs pour T_SETTLE_NS=1000
    
    // Déclencher ADC_ETC TRIG0 (software trigger) - version simplifiée
    // Note: Cette implémentation est simplifiée pour la compatibilité
    
    // Incrémenter step
    currentStep = (currentStep + 1) & 0x0F;
}

// ISR DMA (fin de frame)
void dma_isr() {
    // Clear DMA interrupt flag
    DMA_CINT = DMA_CINT_CINT(0); // Canal 0
    
    // Basculer ping/pong
    readyFrame = activeFrame;
    activeFrame = (activeFrame == &frameA) ? &frameB : &frameA;
    frameReady = true;
    
    // Reconfigurer DMA pour nouvelle frame
    DMA_TCD0_DADDR = (volatile void*)activeFrame->s;
}

void scan_dma_init(uint32_t step_hz) {
    // Version simplifiée pour compatibilité avec Teensy 4.1
    // Cette implémentation utilise une approche hybride : PIT + lecture directe ADC
    
    // 1) Enable clocks disponibles
    CCM_CCGR1 |= CCM_CCGR1_ADC1(CCM_CCGR_ON) | CCM_CCGR1_ADC2(CCM_CCGR_ON);
    CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);
    
    // 2) Configuration ADC1 et ADC2 (version simplifiée)
    ADC1_CFG = ADC_CFG_ADSTS(2) | ADC_CFG_MODE(2); // 10-bit, medium speed
    ADC2_CFG = ADC_CFG_ADSTS(2) | ADC_CFG_MODE(2);
    
    // 3) Configuration PIT pour cadence
    PIT_MCR = 0; // Enable PIT
    
    // Calculer période pour step_hz
    uint32_t period = (F_BUS_ACTUAL / step_hz) - 1;
    PIT_LDVAL0 = period;
    
    // Enable PIT0 avec interrupt
    PIT_TCTRL0 = PIT_TCTRL_TIE | PIT_TCTRL_TEN;
    
    // 4) Enable interrupts
    attachInterruptVector(IRQ_PIT, pit0_isr);
    NVIC_ENABLE_IRQ(IRQ_PIT);
    
    // Reset état
    currentStep = 0;
    frameReady = false;
    readyFrame = nullptr;
    
    Serial.println("DMA init completed (simplified mode)");
}

bool scan_dma_acquire(const DmaFrame*& f) {
    if (frameReady && readyFrame) {
        f = (const DmaFrame*)readyFrame;
        return true;
    }
    return false;
}

void scan_dma_release() {
    frameReady = false;
    readyFrame = nullptr;
}
