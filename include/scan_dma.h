#pragma once
#include <stdint.h>

// 16 positions MUX (Gray), à chaque "step" on lit 8 sorties (MUX1..8) => 128 échantillons / scan complet
constexpr uint8_t  STEPS = 16;
constexpr uint8_t  LINES = 8;    // 8 mux outputs (pins A_MUX1..A_MUX8)
constexpr uint16_t FRAME_SAMPLES = STEPS * LINES; // 128

// Double buffer (ping/pong), 16-bit (résolution 10 bits)
struct DmaFrame {
  uint16_t s[FRAME_SAMPLES]; // ordre [step0:MUX1..8, step1:MUX1..8, ...]
};

void scan_dma_init(uint32_t step_hz);       // configure PIT->XBARA->ADC_ETC->DMA
bool scan_dma_acquire(const DmaFrame*& f);  // renvoie ptr vers frame stable (ping/pong), ou false si rien
void scan_dma_release();                    // libère la frame après traitement
