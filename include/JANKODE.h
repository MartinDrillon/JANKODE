#pragma once
#include <Arduino.h>

// JANKODE - Firmware pour Teensy 4.1 / Clavicore 1.0
// Scanner 120 touches via 8 MUX avec vélocité dv/dt et MIDI Device/Host

// Modes de build disponibles :
// USE_V3_DMA=1 : Mode DMA avec PIT + XBARA + ADC_ETC + eDMA (défaut)
// USE_V3_DMA=0 : Mode SYNC avec lib ADC (fallback)

// Configuration système
#define JANKODE_VERSION "3.0"
#define NUM_KEYS 120
#define SCAN_FREQUENCY_HZ 625  // Fréquence de scan cible

// Includes principaux
#include "keys.h"
#include "keymap.h" 
#include "calibration.h"
#include "midi_routing.h"

#if USE_V3_DMA
#include "scan_dma.h"
#endif

