#include "calibration.h"

CalibrationData calibration;

void calibrationInitDefaults() {
    // Basé sur VCC=3.3V, ADC 10 bits (0-1023)
    // 2.2V ≈ (2.2/3.3) * 1023 ≈ 682 LSB + 30 pour ne pas lancer des suivis involontaires 712
    // 3.08V ≈ (3.08/3.3) * 1023 ≈ 954 LSB - 30 pour éviter de rater un déclenchement 924
    calibration.thresholdLow = 712;      // Seuil de détection pression
    calibration.thresholdHigh = 924;     // Seuil pression confirmée  
    calibration.thresholdRelease = 874;  // thresholdHigh - 50 (hystérésis)
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
