#pragma once
#include "keys.h"

// LUT 120 touches : keymap[mux][channel] -> index dans keyBuf[0..119]
// -1 = trou (MUX1: I4,I6,I9,I11 / MUX8: I1,I3,I12,I14)
extern const int8_t keymap[NUM_MUX][MUX_CHANNELS];

// Fonction pour obtenir l'index d'une touche
inline int8_t getKeyIndex(uint8_t mux, uint8_t channel) {
    if (mux >= NUM_MUX || channel >= MUX_CHANNELS) return -1;
    return keymap[mux][channel];
}

// États des touches pour vélocité
enum KeyState {
    IDLE = 0,
    TRACKING = 1,
    HELD = 2
};

struct KeyData {
    KeyState state;
    uint16_t pressValue;
    uint16_t releaseValue;
    uint32_t pressTime;
    uint32_t releaseTime;
    uint8_t velocity;
    uint8_t note;
    bool noteOn;
};

extern KeyData keyStates[NUM_KEYS];

// Fonctions de traitement
void initKeyMap();         // Initialisation de la map
void processAllKeysOnce();
void processKey(uint8_t keyIndex, uint16_t currentValue);
