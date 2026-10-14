#ifndef THE_STATIONS_BIOMETRIC_ANALYZER_HPP
#define THE_STATIONS_BIOMETRIC_ANALYZER_HPP

#include <vector>
#include <cstdint>

namespace the_stations {

struct SensorData {
    float micVolume{0.0f};        // Scream volume 0.0 - 1.0
    float voiceTremble{0.0f};     // Frequency fluctuation 0.0 - 1.0
    float facialTension{0.0f};    // Facial strain 0.0 - 1.0
    float blinkFrequency{0.0f};   // Blinks per minute relative ratio
    float headTremble{0.0f};      // Head shake amplitude
};

struct BehavioralData {
    float cameraTurnSharpness{0.0f}; // Quick view changes
    float backwardWalkRatio{0.0f};   // Ratio of time walking backward
    float hidingDurationSec{0.0f};   // Time spent crouching/hiding
};

class BiometricAnalyzer {
public:
    BiometricAnalyzer() = default;

    void setSensorsPermitted(bool micPermitted, bool cameraPermitted);
    bool isMicPermitted() const { return m_micPermitted; }
    bool isCameraPermitted() const { return m_cameraPermitted; }

    // Feeds raw frame/audio data to be analyzed strictly in local RAM and discarded immediately
    void processSensorInputInRAM(const SensorData& sensorData);

    // Feeds player behavioral metrics
    void processBehavioralInput(const BehavioralData& behavioralData);

    // Calculate local player fear score (0.0 to 1.0)
    float calculateLocalFearScore() const;

    // Calculate group average fear score for multiplayer DDA
    static float calculateGroupAverageFearScore(const std::vector<float>& playerScores);

private:
    bool m_micPermitted{true};
    bool m_cameraPermitted{true};

    SensorData m_latestSensorData{};
    BehavioralData m_latestBehavioralData{};
};

} // namespace the_stations

#endif // THE_STATIONS_BIOMETRIC_ANALYZER_HPP
