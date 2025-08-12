#pragma once
#include <stdint.h>

// Seuils de calibration basés sur 2.3V/3.2V pour ADC 10 bits
// VCC=3.3V -> 2.3V ≈ 716 LSB, 3.2V ≈ 993 LSB

struct CalibrationData {
    uint16_t thresholdLow;    // Seuil de détection pression (716)
    uint16_t thresholdHigh;   // Seuil pression confirmée (993)
    uint16_t thresholdRelease; // Seuil de relâchement (thresholdHigh - 50)
};

extern CalibrationData calibration;

// Initialisation avec valeurs par défaut
void calibrationInitDefaults();

// Fonctions de calibration (pour extensions futures)
void startCalibration();
bool isCalibrationComplete();
void applyCalibration();
