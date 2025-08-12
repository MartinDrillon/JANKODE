#include "calibration.h"

CalibrationData calibration;

void calibrationInitDefaults() {
    // Basé sur VCC=3.3V, ADC 10 bits (0-1023)
    // 2.3V ≈ (2.3/3.3) * 1023 ≈ 716 LSB
    // 3.2V ≈ (3.2/3.3) * 1023 ≈ 993 LSB
    calibration.thresholdLow = 716;      // Seuil de détection pression
    calibration.thresholdHigh = 993;     // Seuil pression confirmée  
    calibration.thresholdRelease = 943;  // thresholdHigh - 50 (hystérésis)
}

void startCalibration() {
    // TODO: Implémentation future pour calibration automatique
}

bool isCalibrationComplete() {
    // TODO: Logique de completion de calibration
    return true; // Pour l'instant, toujours calibré
}

void applyCalibration() {
    // TODO: Application des paramètres calibrés
}
