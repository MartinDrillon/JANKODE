#include <Arduino.h>
#include "keys.h"
#include "keymap.h"
#include "calibration.h"
#include "midi_routing.h"

#if USE_V3_DMA
#include "scan_dma.h"
#else
void initSyncMode();
void scanOnceSync();
#endif

void setup() {
    Serial.begin(115200);
    
    // Initialisation des pins
    initPins();
    
    // Initialisation de la map d'écriture et keymap
    initWriteMap();
    initKeyMap();
    
    // Initialisation de la calibration
    calibrationInitDefaults();
    
    // Initialisation MIDI
    initMidi();
    
    // Initialisation du mode de scan
#if USE_V3_DMA
    // Mode DMA : PIT + XBARA + ADC_ETC + eDMA
    scan_dma_init(10000); // 10 kHz steps ≈ 625 scans/s
    Serial.println("JANKODE V3 DMA Mode initialized");
#else
    // Mode SYNC : fallback via lib ADC
    initSyncMode();
    Serial.println("JANKODE V2 SYNC Mode initialized");
#endif
    
    Serial.println("Clavicore 1.0 ready - 120 keys scanner");
}

void loop() {
    // Mise à jour du mode MIDI (lecture pins 4/5)
    static uint32_t lastMidiCheck = 0;
    if (millis() - lastMidiCheck > 1000) { // Check toutes les secondes
        updateMidiMode();
        lastMidiCheck = millis();
    }
    
    // Acquisition des données selon le mode
#if USE_V3_DMA
    // Mode DMA : récupération frame prête
    const DmaFrame* f;
    if (scan_dma_acquire(f)) {
        // Déversement vers keyBuf via table W
        for (uint8_t si = 0; si < 16; ++si) {
            uint8_t ch = GRAY[si];
            const uint16_t* row = &f->s[si * 8];
            
            // MUX1..8 dans l'ordre
            *W[0][ch] = row[0]; *W[1][ch] = row[1]; 
            *W[2][ch] = row[2]; *W[3][ch] = row[3];
            *W[4][ch] = row[4]; *W[5][ch] = row[5]; 
            *W[6][ch] = row[6]; *W[7][ch] = row[7];
        }
        scan_dma_release();
    }
#else
    // Mode SYNC : scan logiciel
    scanOnceSync();
#endif
    
    // Service USB Host
    midiHostTask();
    
    // Traitement détection & vélocité sur keyBuf[0..119]
    processAllKeysOnce();
    
    // Lecture MIDI entrants (device)
    while (usbMIDI.read()) {
        // Traitement des messages MIDI entrants si nécessaire
    }
}
