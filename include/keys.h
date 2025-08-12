#pragma once
#include <Arduino.h>

// Configuration des pins pour les MUX
// Sélecteurs GROUP A (MUX1-4) : pins 27,26,28,29 (S0,S1,S2,S3)
constexpr uint8_t SEL_A_S0 = 27;
constexpr uint8_t SEL_A_S1 = 26;
constexpr uint8_t SEL_A_S2 = 28;
constexpr uint8_t SEL_A_S3 = 29;

// Sélecteurs GROUP B (MUX5-8) : pins 38,39,37,36 (S0,S1,S2,S3)
constexpr uint8_t SEL_B_S0 = 38;
constexpr uint8_t SEL_B_S1 = 39;
constexpr uint8_t SEL_B_S2 = 37;
constexpr uint8_t SEL_B_S3 = 36;

// Entrées analogiques MUX1-8 : pins 33,32,31,30,45,44,43,42
constexpr uint8_t A_MUX1 = 33;  // A7
constexpr uint8_t A_MUX2 = 32;  // A8
constexpr uint8_t A_MUX3 = 31;  // A9
constexpr uint8_t A_MUX4 = 30;  // A10
constexpr uint8_t A_MUX5 = 45;  // A11
constexpr uint8_t A_MUX6 = 44;  // A12
constexpr uint8_t A_MUX7 = 43;  // A13
constexpr uint8_t A_MUX8 = 42;  // A14

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
