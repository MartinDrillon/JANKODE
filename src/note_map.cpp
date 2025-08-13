#include "note_map.h"
#include "keymap.h"

// Note map: MIDI note pour chaque position [mux][channel]
// Format: noteMap[mux][channel] = MIDI_NOTE ou -1 pour les trous
// Toutes les touches valides jouent C4 (MIDI 60) pour l'instant

const int8_t noteMap[NUM_MUX][MUX_CHANNELS] = {
    {35, 38, 36, 37, -1, 34, -1, 33, 32, -1, 31, -1, 36, 33, 35, 34}, // MUX1→Key: 0,1,2,3,_,4,_,5,6,_,7,_,8,9,10,11
    
    {43, 46, 44, 45, 39, 42, 40, 41, 40, 37, 39, 38, 44, 41, 43, 42}, // MUX2→Key: 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27
    
    {51, 54, 52, 53, 47, 50, 48, 49, 48, 45, 47, 46, 52, 49, 51, 50}, // MUX3→Key: 28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43
    
    {59, 62, 60, 61, 55, 58, 56, 57, 56, 53, 55, 54, 60, 57, 59, 58}, // MUX4→Key: 44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59
    
    {67, 70, 68, 69, 63, 66, 64, 65, 64, 61, 63, 62, 68, 65, 67, 66}, // MUX5→Key: 60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75
    
    {75, 78, 76, 77, 71, 74, 72, 73, 72, 69, 71, 70, 76, 73, 75, 74}, // MUX6→Key: 76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91
    
    {83, 86, 84, 85, 79, 82, 80, 81, 80, 77, 79, 78, 84, 81, 83, 82}, // MUX7→Key: 92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107
    
    {91, -1, 92, -1, 87, 90, 88, 89, 88, 85, 87, 86, -1, 89, -1, 90}  // MUX8→Key: 108,_,109,_,110,111,112,113,114,115,116,117,_,118,_,119
};

int8_t getNoteForKey(uint8_t keyIndex) {
    // Parcours de la keymap pour trouver la position [mux][channel] du keyIndex
    for (uint8_t mux = 0; mux < NUM_MUX; mux++) {
        for (uint8_t ch = 0; ch < MUX_CHANNELS; ch++) {
            if (keymap[mux][ch] == keyIndex) {
                return noteMap[mux][ch];
            }
        }
    }
    return -1; // Key index non trouvé
}

int8_t getNoteForMuxPosition(uint8_t mux, uint8_t channel) {
    if (mux >= NUM_MUX || channel >= MUX_CHANNELS) return -1;
    return noteMap[mux][channel];
}
