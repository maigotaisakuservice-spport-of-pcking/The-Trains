#ifndef THE_STATIONS_TEXTURE_GENERATOR_HPP
#define THE_STATIONS_TEXTURE_GENERATOR_HPP

#include <vector>
#include <cstdint>
#include <cstddef>

namespace the_stations {

struct TextureBuffer {
    uint32_t width;
    uint32_t height;
    std::vector<uint32_t> pixelsRGBA; // RGBA uint32 pixels
};

class TextureGenerator {
public:
    TextureGenerator() = default;

    // 1. Concrete dirty/rust/crack texture (512x512)
    static TextureBuffer generateDirtyConcreteTexture(uint32_t width = 512, uint32_t height = 512, uint32_t seed = 42);

    // 2. Station platform tile & bloodstain texture (512x512)
    static TextureBuffer generatePlatformTileBloodTexture(uint32_t width = 512, uint32_t height = 512, uint32_t seed = 123);

    // 3. Film grain Perlin noise texture (256x256)
    static TextureBuffer generatePerlinNoiseGrainTexture(uint32_t width = 256, uint32_t height = 256, uint32_t seed = 777);

private:
    static float perlinNoise2D(float x, float y, uint32_t seed);
    static float grad2D(int hash, float x, float y);
};

} // namespace the_stations

#endif // THE_STATIONS_TEXTURE_GENERATOR_HPP
