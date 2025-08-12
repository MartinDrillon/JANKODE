#include "midi_routing.h"
#include "keys.h"

USBHost myusb;
MIDIDevice midi1(myusb);
MidiMode currentMidiMode = DEVICE_ONLY;

void initMidi() {
    // Initialisation USB Host
    myusb.begin();
    
    // Détection du mode MIDI initial
    updateMidiMode();
    
    // Initialisation usbMIDI (device) - toujours actif
    // Note: usbMIDI est automatiquement initialisé par le core Teensy
}

void updateMidiMode() {
    // Lecture des pins 4 & 5 pour déterminer le mode
    bool pin4 = digitalRead(MIDI_PIN4) == HIGH;
    bool pin5 = digitalRead(MIDI_PIN5) == HIGH;
    
    if (!pin4 && !pin5) {
        currentMidiMode = DEVICE_ONLY;        // 4=LOW, 5=LOW
    } else if (pin4 && !pin5) {
        currentMidiMode = HOST_ONLY;          // 4=HIGH, 5=LOW
    } else if (!pin4 && pin5) {
        currentMidiMode = DEVICE_ONLY_ALT;    // 4=LOW, 5=HIGH (même que DEVICE_ONLY)
    } else {
        currentMidiMode = HOST_AND_DEVICE;    // 4=HIGH, 5=HIGH
    }
}

void midiHostTask() {
    // Tâche USB Host (obligatoire pour USBHost_t36)
    myusb.Task();
    
    // Traitement des messages MIDI Host entrants (si nécessaire)
    if (midi1) {
        // Lecture des messages MIDI du périphérique connecté
        // TODO: Implémenter le routage des messages entrants si nécessaire
    }
}

void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
    switch (currentMidiMode) {
        case DEVICE_ONLY:
        case DEVICE_ONLY_ALT:
            usbMIDI.sendNoteOn(note, velocity, channel);
            break;
            
        case HOST_ONLY:
            if (midi1) {
                midi1.sendNoteOn(note, velocity, channel);
            }
            break;
            
        case HOST_AND_DEVICE:
            usbMIDI.sendNoteOn(note, velocity, channel);
            if (midi1) {
                midi1.sendNoteOn(note, velocity, channel);
            }
            break;
    }
}

void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel) {
    switch (currentMidiMode) {
        case DEVICE_ONLY:
        case DEVICE_ONLY_ALT:
            usbMIDI.sendNoteOff(note, velocity, channel);
            break;
            
        case HOST_ONLY:
            if (midi1) {
                midi1.sendNoteOff(note, velocity, channel);
            }
            break;
            
        case HOST_AND_DEVICE:
            usbMIDI.sendNoteOff(note, velocity, channel);
            if (midi1) {
                midi1.sendNoteOff(note, velocity, channel);
            }
            break;
    }
}

void sendControlChange(uint8_t controller, uint8_t value, uint8_t channel) {
    switch (currentMidiMode) {
        case DEVICE_ONLY:
        case DEVICE_ONLY_ALT:
            usbMIDI.sendControlChange(controller, value, channel);
            break;
            
        case HOST_ONLY:
            if (midi1) {
                midi1.sendControlChange(controller, value, channel);
            }
            break;
            
        case HOST_AND_DEVICE:
            usbMIDI.sendControlChange(controller, value, channel);
            if (midi1) {
                midi1.sendControlChange(controller, value, channel);
            }
            break;
    }
}
