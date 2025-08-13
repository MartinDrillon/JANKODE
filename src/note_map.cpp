#include "note_map.h"
#include "keymap.h"

// Note map: MIDI note pour chaque position [mux][channel]
// Format: noteMap[mux][channel] = MIDI_NOTE ou -1 pour les trous
// Toutes les touches valides jouent C4 (MIDI 60) pour l'instant

const int8_t noteMap[NUM_MUX][MUX_CHANNELS] = {
    // MUX1 (index 0) - trous aux positions 4,6,9,11
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, -1, 60, -1, 60, 60, -1, 60, -1, 60, 60, 60, 60}, // MUX1→Key: 0,1,2,3,_,4,_,5,6,_,7,_,8,9,10,11
    
    // MUX2 (index 1) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX2→Key: 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27
    
    // MUX3 (index 2) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX3→Key: 28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43
    
    // MUX4 (index 3) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX4→Key: 44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59
    
    // MUX5 (index 4) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX5→Key: 60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75
    
    // MUX6 (index 5) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX6→Key: 76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91
    
    // MUX7 (index 6) - toutes les positions valides
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // MUX7→Key: 92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107
    
    // MUX8 (index 7) - trous aux positions 1,3,12,14
    // CH:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {    60, -1, 60, -1, 60, 60, 60, 60, 60, 60, 60, 60, -1, 60, -1, 60}  // MUX8→Key: 108,_,109,_,110,111,112,113,114,115,116,117,_,118,_,119
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
