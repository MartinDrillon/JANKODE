#pragma once
#include "keys.h"

// Note map: attribution MIDI pour chaque position [mux][channel]
// -1 = trou (pas de note assignée)
// 60 = C4 (Middle C) pour toutes les touches valides

extern const int8_t noteMap[NUM_MUX][MUX_CHANNELS];

// Helper function to get MIDI note for a key index
int8_t getNoteForKey(uint8_t keyIndex);

// Helper function to get MIDI note for MUX position
int8_t getNoteForMuxPosition(uint8_t mux, uint8_t channel);
