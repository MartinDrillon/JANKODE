#include "keys.h"
#include "calibration.h"
#include "midi_routing.h"
#include <ADC.h>

ADC *adc = new ADC();

// Pins analogiques dans l'ordre MUX1-8
static const uint8_t analogPins[8] = {A_MUX1, A_MUX2, A_MUX3, A_MUX4, A_MUX5, A_MUX6, A_MUX7, A_MUX8};

void initSyncMode() {
    // Configuration ADC pour mode synchrone
    adc->adc0->setAveraging(1);
    adc->adc0->setResolution(10);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
    
    adc->adc1->setAveraging(1);
    adc->adc1->setResolution(10);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
}

void scanOnceSync() {
    // Scan de toutes les positions Gray
    for (uint8_t step = 0; step < 16; step++) {
        uint8_t grayCode = GRAY[step];
        
        // Set sélecteurs MUX
        digitalWrite(SEL_A_S0, grayCode & 1);
        digitalWrite(SEL_A_S1, (grayCode >> 1) & 1);
        digitalWrite(SEL_A_S2, (grayCode >> 2) & 1);
        digitalWrite(SEL_A_S3, (grayCode >> 3) & 1);
        
        digitalWrite(SEL_B_S0, grayCode & 1);
        digitalWrite(SEL_B_S1, (grayCode >> 1) & 1);
        digitalWrite(SEL_B_S2, (grayCode >> 2) & 1);
        digitalWrite(SEL_B_S3, (grayCode >> 3) & 1);
        
        // Délai de stabilisation
        delayMicroseconds(1); // 1µs pour T_SETTLE_NS=1000
        
        // Lecture simultanée des 8 MUX
        ADC::Sync_result result = adc->analogSynchronizedRead(A_MUX1, A_MUX5);
        uint16_t values[8];
        
        values[0] = result.result_adc0; // MUX1
        values[4] = result.result_adc1; // MUX5
        
        // Lectures individuelles pour les autres (fallback)
        values[1] = adc->adc0->analogRead(A_MUX2);
        values[2] = adc->adc0->analogRead(A_MUX3);
        values[3] = adc->adc0->analogRead(A_MUX4);
        values[5] = adc->adc1->analogRead(A_MUX6);
        values[6] = adc->adc1->analogRead(A_MUX7);
        values[7] = adc->adc1->analogRead(A_MUX8);
        
        // Écriture dans keyBuf via table W (sink write pour trous)
        for (uint8_t mux = 0; mux < 8; mux++) {
            *W[mux][grayCode] = values[mux];
        }
    }
}
