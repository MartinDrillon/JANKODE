#pragma once
#include <Arduino.h>

// Configuration des pins pour les MUX (NOUVELLE CARTE)
// Sélecteurs GROUP A (MUX1-4) : pins 35,34,36,37 (S0,S1,S2,S3)
constexpr uint8_t SEL_A_S0 = 35;
constexpr uint8_t SEL_A_S1 = 34;
constexpr uint8_t SEL_A_S2 = 36;
constexpr uint8_t SEL_A_S3 = 37;
// Tableau pratique (ordre S0,S1,S2,S3)
constexpr uint8_t SEL_A[4] = { SEL_A_S0, SEL_A_S1, SEL_A_S2, SEL_A_S3 };

// Sélecteurs GROUP B (MUX5-8) : pins 17,16,18,19 (S0,S1,S2,S3)
constexpr uint8_t SEL_B_S0 = 17;
constexpr uint8_t SEL_B_S1 = 16;
constexpr uint8_t SEL_B_S2 = 18;
constexpr uint8_t SEL_B_S3 = 19;
constexpr uint8_t SEL_B[4] = { SEL_B_S0, SEL_B_S1, SEL_B_S2, SEL_B_S3 };

// Entrées analogiques MUX1-8 : nouvelle correspondance
// MUX1..8 -> pins 41,40,39,38,20,21,22,23
constexpr uint8_t A_MUX1 = 41;  // anciennement 33
constexpr uint8_t A_MUX2 = 40;  // anciennement 32
constexpr uint8_t A_MUX3 = 39;  // anciennement 31
constexpr uint8_t A_MUX4 = 38;  // anciennement 30
constexpr uint8_t A_MUX5 = 20;  // anciennement 45
constexpr uint8_t A_MUX6 = 21;  // anciennement 44
constexpr uint8_t A_MUX7 = 22;  // anciennement 43
constexpr uint8_t A_MUX8 = 23;  // anciennement 42

// Pins MIDI routing
constexpr uint8_t MIDI_PIN4 = 4;
constexpr uint8_t MIDI_PIN5 = 5;

// Code Gray pour séquence MUX (anti-glitch)
extern const uint8_t GRAY[16];

// Configuration 120 touches
constexpr uint8_t NUM_KEYS = 120;
constexpr uint8_t NUM_MUX = 8;
constexpr uint8_t MUX_CHANNELS = 16;

// Buffer principal des valeurs de touches
extern volatile uint16_t keyBuf[NUM_KEYS];
extern volatile uint16_t dummySink;

// Map d'écriture pour sink write (évite les branches)
extern volatile uint16_t* W[NUM_MUX][MUX_CHANNELS];

// Fonctions d'initialisation
void initPins();
void initWriteMap();
