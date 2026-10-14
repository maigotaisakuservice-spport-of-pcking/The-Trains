#ifndef THE_STATIONS_AUDIO_DSP_HPP
#define THE_STATIONS_AUDIO_DSP_HPP

#include <vector>
#include <cstdint>
#include <string>

namespace the_stations {

struct AudioPcmBuffer {
    uint32_t sampleRate{44100};
    uint16_t numChannels{1};
    std::vector<int16_t> pcmSamples;
};

class AudioDSPGenerator {
public:
    AudioDSPGenerator() = default;

    // 1. Train movement audio synthesis (low-pass rumble + doppler modulation)
    static AudioPcmBuffer generateTrainRumbleDopplerPcm(float durationSec = 3.0f, uint32_t sampleRate = 44100);

    // 2. Creepy wind ambience synthesis (bandpass filtered noise)
    static AudioPcmBuffer generateCreepyWindAmbiencePcm(float durationSec = 3.0f, uint32_t sampleRate = 44100);

    // 3. Station announcement dynamic chime waveform synthesis
    static AudioPcmBuffer generateStationAnnouncementChimePcm(const std::string& textPrompt, uint32_t sampleRate = 44100);
};

} // namespace the_stations

#endif // THE_STATIONS_AUDIO_DSP_HPP
