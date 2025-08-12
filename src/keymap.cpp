#include "keymap.h"
#include "keys.h"
#include "calibration.h"
#include "midi_routing.h"

// LUT 120 touches : keymap[mux][channel] -> index dans keyBuf[0..119]
// -1 = trou (MUX1: I4,I6,I9,I11 / MUX8: I1,I3,I12,I14)
const int8_t keymap[NUM_MUX][MUX_CHANNELS] = {
    // MUX1 (index 0) - trous aux positions 4,6,9,11
    { 0,  1,  2,  3, -1,  5, -1,  7,  8, -1, 10, -1, 12, 13, 14, 15},
    // MUX2 (index 1)
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
    // MUX3 (index 2) 
    {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
    // MUX4 (index 3)
    {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
    // MUX5 (index 4)
    {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    // MUX6 (index 5)
    {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
    // MUX7 (index 6)
    {96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111},
    // MUX8 (index 7) - trous aux positions 1,3,12,14
    {112, -1,114, -1,116,117,118,119, -1, -1, -1, -1, -1, -1, -1, -1}
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
        keyStates[i].note = 36 + i; // Notes MIDI à partir de C2
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
