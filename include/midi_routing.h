#pragma once
#include <Arduino.h>
#include <USBHost_t36.h>

// Gestion du routage MIDI selon pins 4 & 5
enum MidiMode {
    DEVICE_ONLY = 0,    // 4=LOW, 5=LOW
    HOST_ONLY = 1,      // 4=HIGH, 5=LOW  
    DEVICE_ONLY_ALT = 2, // 4=LOW, 5=HIGH (même que DEVICE_ONLY)
    HOST_AND_DEVICE = 3  // 4=HIGH, 5=HIGH
};

extern USBHost myusb;
extern MIDIDevice midi1;
extern MidiMode currentMidiMode;

// Initialisation MIDI
void initMidi();
void updateMidiMode();

// Tâche USB Host (à appeler dans loop)
void midiHostTask();

// Envoi de messages MIDI
void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel = 1);
void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel = 1);
void sendControlChange(uint8_t controller, uint8_t value, uint8_t channel = 1);
