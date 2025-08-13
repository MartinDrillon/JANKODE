#include "keymap.h"
#include "keys.h"
#include "calibration.h"
#include "midi_routing.h"
#include "note_map.h"

// LUT 128 touches : keymap[mux][channel] -> index dans keyBuf[0..127]
// -1 = trou (MUX1: I4,I6,I9,I11 / MUX8: I1,I3,I12,I14)
const int8_t keymap[NUM_MUX][MUX_CHANNELS] = {
    // MUX1 (index 0) - trous aux positions 4,6,9,11
    { 0,  1,  2,  3, -1,  4, -1,  5,  6, -1,  7, -1,  8,  9, 10, 11},
    // MUX2 (index 1)
    {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
    // MUX3 (index 2) 
    {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43},
    // MUX4 (index 3)
    {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
    // MUX5 (index 4)
    {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75},
    // MUX6 (index 5)
    {76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91},
    // MUX7 (index 6)
    {92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107},
    // MUX8 (index 7) - trous aux positions 1,3,12,14
    {108, -1,109, -1,110,111,112,113,114,115,116,117, -1,118, -1,119}
};

KeyData keyStates[NUM_KEYS];

void initKeyMap() {
    // Mise à jour de la map d'écriture W selon la LUT
    for (int mux = 0; mux < NUM_MUX; mux++) {
        for (int ch = 0; ch < MUX_CHANNELS; ch++) {
            int8_t keyIndex = keymap[mux][ch];
            if (keyIndex >= 0 && keyIndex < NUM_KEYS) {
                W[mux][ch] = &keyBuf[keyIndex];
            } else {
                W[mux][ch] = &dummySink; // Sink write pour les trous
            }
        }
    }
    
    // Initialisation des états des touches
    for (int i = 0; i < NUM_KEYS; i++) {
        keyStates[i].state = IDLE;
        keyStates[i].pressValue = 0;
        keyStates[i].releaseValue = 0;
        keyStates[i].pressTime = 0;
        keyStates[i].releaseTime = 0;
        keyStates[i].velocity = 0;
        keyStates[i].note = getNoteForKey(i); // Note depuis note_map
        keyStates[i].noteOn = false;
    }
}

void processAllKeysOnce() {
    for (int i = 0; i < NUM_KEYS; i++) {
        processKey(i, keyBuf[i]);
    }
}

void processKey(uint8_t keyIndex, uint16_t currentValue) {
    if (keyIndex >= NUM_KEYS) return;
    
    KeyData& key = keyStates[keyIndex];
    uint32_t currentTime = micros();
    
    switch (key.state) {
        case IDLE:
            if (currentValue > calibration.thresholdLow) {
                key.state = TRACKING;
                key.pressValue = currentValue;
                key.pressTime = currentTime;
            }
            break;
            
        case TRACKING:
            if (currentValue > calibration.thresholdHigh) {
                // Touche enfoncée - calcul vélocité
                key.state = HELD;
                key.releaseValue = currentValue;
                
                // Calcul vélocité dv/dt
                uint32_t deltaTime = currentTime - key.pressTime;
                uint16_t deltaValue = currentValue - key.pressValue;
                
                if (deltaTime > 0) {
                    // Normalisation vélocité MIDI (1-127)
                    uint32_t velocity = (deltaValue * 1000000UL) / deltaTime; // dv/dt en LSB/s
                    velocity = velocity / 10000; // Scaling empirique
                    if (velocity < 1) velocity = 1;
                    if (velocity > 127) velocity = 127;
                    key.velocity = velocity;
                } else {
                    key.velocity = 64; // Vélocité par défaut
                }
                
                // Envoi Note On
                sendNoteOn(key.note, key.velocity);
                key.noteOn = true;
                
            } else if (currentValue < calibration.thresholdLow) {
                // Retour à IDLE sans déclenchement
                key.state = IDLE;
            }
            break;
            
        case HELD:
            if (currentValue < calibration.thresholdRelease) {
                // Touche relâchée
                key.state = IDLE;
                
                if (key.noteOn) {
                    sendNoteOff(key.note, key.velocity);
                    key.noteOn = false;
                }
            }
            break;
    }
}
