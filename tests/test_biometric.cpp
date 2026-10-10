#include <iostream>
#include <cassert>
#include "shared/biometric/biometric_analyzer.hpp"

using namespace the_stations;

int main() {
    BiometricAnalyzer analyzer;

    // Sensor active testing
    SensorData sensor{0.8f, 0.7f, 0.9f, 0.5f, 0.6f};
    BehavioralData behavioral{0.3f, 0.2f, 10.0f};

    analyzer.processSensorInputInRAM(sensor);
    analyzer.processBehavioralInput(behavioral);

    float scoreWithSensors = analyzer.calculateLocalFearScore();
    assert(scoreWithSensors > 0.5f);

    // Fallback testing (Sensors denied)
    analyzer.setSensorsPermitted(false, false);
    float fallbackScore = analyzer.calculateLocalFearScore();
    assert(fallbackScore > 0.0f);

    // Group average testing
    std::vector<float> scores = {0.2f, 0.4f, 0.6f, 0.8f};
    float avg = BiometricAnalyzer::calculateGroupAverageFearScore(scores);
    assert(std::abs(avg - 0.5f) < 0.001f);

    std::cout << "[PASS] Biometric & DDA Fallback Test" << std::endl;
    return 0;
}
