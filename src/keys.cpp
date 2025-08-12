#include "keys.h"

// Code Gray pour séquence anti-glitch
const uint8_t GRAY[16] = {0,1,3,2,6,7,5,4,12,13,15,14,10,11,9,8};

// Buffer principal des valeurs de touches
volatile uint16_t keyBuf[NUM_KEYS];
volatile uint16_t dummySink;

// Map d'écriture pour sink write (évite les branches)
volatile uint16_t* W[NUM_MUX][MUX_CHANNELS];

void initPins() {
    // Configuration des pins sélecteurs MUX
    pinMode(SEL_A_S0, OUTPUT);
    pinMode(SEL_A_S1, OUTPUT);
    pinMode(SEL_A_S2, OUTPUT);
    pinMode(SEL_A_S3, OUTPUT);
    
    pinMode(SEL_B_S0, OUTPUT);
    pinMode(SEL_B_S1, OUTPUT);
    pinMode(SEL_B_S2, OUTPUT);
    pinMode(SEL_B_S3, OUTPUT);
    
    // Configuration des pins MIDI routing
    pinMode(MIDI_PIN4, INPUT_PULLUP);
    pinMode(MIDI_PIN5, INPUT_PULLUP);
    
    // Initialisation des sélecteurs à 0
    digitalWrite(SEL_A_S0, LOW);
    digitalWrite(SEL_A_S1, LOW);
    digitalWrite(SEL_A_S2, LOW);
    digitalWrite(SEL_A_S3, LOW);
    
    digitalWrite(SEL_B_S0, LOW);
    digitalWrite(SEL_B_S1, LOW);
    digitalWrite(SEL_B_S2, LOW);
    digitalWrite(SEL_B_S3, LOW);
}

void initWriteMap() {
    // Initialisation du buffer des touches
    for (int i = 0; i < NUM_KEYS; i++) {
        keyBuf[i] = 0;
    }
    
    // Configuration de la map d'écriture avec sink write pour les trous
    // Cette table sera remplie dans keymap.cpp avec les vraies correspondances
    for (int mux = 0; mux < NUM_MUX; mux++) {
        for (int ch = 0; ch < MUX_CHANNELS; ch++) {
            W[mux][ch] = &dummySink; // Par défaut, écrit dans sink
        }
    }
}
