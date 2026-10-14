#include "audio_dsp.hpp"
#include <cmath>
#include <random>
#include <algorithm>

namespace the_stations {

static constexpr float PI = 3.14159265358979323846f;

AudioPcmBuffer AudioDSPGenerator::generateTrainRumbleDopplerPcm(float durationSec, uint32_t sampleRate) {
    size_t totalSamples = static_cast<size_t>(durationSec * sampleRate);
    AudioPcmBuffer buffer{sampleRate, 1, std::vector<int16_t>(totalSamples)};

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> whiteNoise(-1.0f, 1.0f);

    float lowPassState = 0.0f;

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;

        // Doppler frequency modulation shift
        float dopplerFreq = 60.0f + 30.0f * std::sin(2.0f * PI * 0.5f * t);
        float tone = std::sin(2.0f * PI * dopplerFreq * t);

        // Low-pass rumble filter
        float rawNoise = whiteNoise(rng);
        lowPassState += 0.1f * (rawNoise - lowPassState);

        float mixedSample = tone * 0.4f + lowPassState * 0.6f;
        int16_t pcmVal = static_cast<int16_t>(std::clamp(mixedSample * 16000.0f, -32768.0f, 32767.0f));
        buffer.pcmSamples[i] = pcmVal;
    }
    return buffer;
}

AudioPcmBuffer AudioDSPGenerator::generateCreepyWindAmbiencePcm(float durationSec, uint32_t sampleRate) {
    size_t totalSamples = static_cast<size_t>(durationSec * sampleRate);
    AudioPcmBuffer buffer{sampleRate, 1, std::vector<int16_t>(totalSamples)};

    std::mt19937 rng(123);
    std::uniform_real_distribution<float> whiteNoise(-1.0f, 1.0f);

    float bandPassState1 = 0.0f;
    float bandPassState2 = 0.0f;

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;

        // Wind swell modulation
        float swell = 0.5f + 0.5f * std::sin(2.0f * PI * 0.2f * t);

        float noise = whiteNoise(rng);
        bandPassState1 += 0.05f * (noise - bandPassState1);
        bandPassState2 += 0.02f * (bandPassState1 - bandPassState2);

        float mixed = (bandPassState1 - bandPassState2) * swell;
        int16_t pcmVal = static_cast<int16_t>(std::clamp(mixed * 24000.0f, -32768.0f, 32767.0f));
        buffer.pcmSamples[i] = pcmVal;
    }
    return buffer;
}

AudioPcmBuffer AudioDSPGenerator::generateStationAnnouncementChimePcm(const std::string& textPrompt, uint32_t sampleRate) {
    // Ping-Pong-Pan-Pon 4-tone chime synthesis
    float durationSec = 2.0f;
    size_t totalSamples = static_cast<size_t>(durationSec * sampleRate);
    AudioPcmBuffer buffer{sampleRate, 1, std::vector<int16_t>(totalSamples)};

    float freqs[4] = {523.25f, 659.25f, 783.99f, 1046.50f}; // C5, E5, G5, C6

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        int toneIdx = std::clamp(static_cast<int>(t / 0.5f), 0, 3);

        float toneT = fmod(t, 0.5f);
        float envelope = std::exp(-5.0f * toneT); // Decay
        float val = std::sin(2.0f * PI * freqs[toneIdx] * t) * envelope;

        int16_t pcmVal = static_cast<int16_t>(std::clamp(val * 28000.0f, -32768.0f, 32767.0f));
        buffer.pcmSamples[i] = pcmVal;
    }
    return buffer;
}

} // namespace the_stations
