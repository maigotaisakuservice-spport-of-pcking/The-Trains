#include "biometric_analyzer.hpp"
#include <algorithm>
#include <numeric>

namespace the_stations {

void BiometricAnalyzer::setSensorsPermitted(bool micPermitted, bool cameraPermitted) {
    m_micPermitted = micPermitted;
    m_cameraPermitted = cameraPermitted;
}

void BiometricAnalyzer::processSensorInputInRAM(const SensorData& sensorData) {
    // Process in RAM and overwrite immediately
    m_latestSensorData = sensorData;
}

void BiometricAnalyzer::processBehavioralInput(const BehavioralData& behavioralData) {
    m_latestBehavioralData = behavioralData;
}

float BiometricAnalyzer::calculateLocalFearScore() const {
    float fearScore = 0.0f;

    // If camera or mic permissions are granted, calculate sensor fear score
    if (m_micPermitted || m_cameraPermitted) {
        float sensorScore = 0.0f;
        int activeSensors = 0;

        if (m_micPermitted) {
            sensorScore += (m_latestSensorData.micVolume * 0.6f + m_latestSensorData.voiceTremble * 0.4f);
            activeSensors++;
        }
        if (m_cameraPermitted) {
            sensorScore += (m_latestSensorData.facialTension * 0.4f +
                            m_latestSensorData.blinkFrequency * 0.3f +
                            m_latestSensorData.headTremble * 0.3f);
            activeSensors++;
        }

        fearScore += (sensorScore / activeSensors) * 0.6f;
    }

    // Behavioral fallback calculation
    float behavioralScore = std::min(1.0f,
        m_latestBehavioralData.cameraTurnSharpness * 0.4f +
        m_latestBehavioralData.backwardWalkRatio * 0.3f +
        (m_latestBehavioralData.hidingDurationSec / 60.0f) * 0.3f
    );

    if (!m_micPermitted && !m_cameraPermitted) {
        // Full fallback to behavioral data
        fearScore = behavioralScore;
    } else {
        // Combined score
        fearScore += behavioralScore * 0.4f;
    }

    return std::clamp(fearScore, 0.0f, 1.0f);
}

float BiometricAnalyzer::calculateGroupAverageFearScore(const std::vector<float>& playerScores) {
    if (playerScores.empty()) return 0.0f;
    float sum = std::accumulate(playerScores.begin(), playerScores.end(), 0.0f);
    return sum / playerScores.size();
}

} // namespace the_stations
