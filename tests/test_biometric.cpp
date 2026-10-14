#include <iostream>
#include <cassert>
#include "shared/biometric/biometric_analyzer.hpp"

using namespace the_stations;

int main() {
    BiometricAnalyzer analyzer;

    // 1. DDA Fallback Test (Camera and Mic permissions denied)
    analyzer.setSensorsPermitted(false, false);
    assert(!analyzer.isMicPermitted());
    assert(!analyzer.isCameraPermitted());

    // Feed purely behavioral data
    BehavioralData behavioral{0.8f, 0.6f, 45.0f}; // Sharp turns, backing walk, 45s crouching
    analyzer.processBehavioralInput(behavioral);

    float fallbackScore = analyzer.calculateLocalFearScore();
    assert(fallbackScore > 0.6f); // Ensure fallback score calculates correctly without sensors

    // Group average DDA test
    std::vector<float> groupScores = {0.8f, 0.6f, 0.9f, 0.7f};
    float avg = BiometricAnalyzer::calculateGroupAverageFearScore(groupScores);
    assert(std::abs(avg - 0.75f) < 0.001f);

    std::cout << "[PASS] DDA Analyzer Sensor Denied Fallback Test" << std::endl;
    return 0;
}
