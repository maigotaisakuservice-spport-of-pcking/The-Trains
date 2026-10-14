#include "texture_generator.hpp"
#include <cmath>
#include <random>
#include <algorithm>

namespace the_stations {

float TextureGenerator::grad2D(int hash, float x, float y) {
    int h = hash & 7;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float TextureGenerator::perlinNoise2D(float x, float y, uint32_t seed) {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
    float v = y * y * y * (y * (y * 6.0f - 15.0f) + 10.0f);

    std::mt19937 rng(seed + X * 57 + Y);
    int p[512];
    for (int i = 0; i < 256; ++i) p[i] = i;
    std::shuffle(p, p + 256, rng);
    for (int i = 0; i < 256; ++i) p[256 + i] = p[i];

    int A = p[X] + Y;
    int B = p[X + 1] + Y;

    float g1 = grad2D(p[A], x, y);
    float g2 = grad2D(p[B], x - 1.0f, y);
    float g3 = grad2D(p[A + 1], x, y - 1.0f);
    float g4 = grad2D(p[B + 1], x - 1.0f, y - 1.0f);

    float nx1 = g1 + u * (g2 - g1);
    float nx2 = g3 + u * (g4 - g3);

    return (nx1 + v * (nx2 - nx1) + 1.0f) * 0.5f;
}

TextureBuffer TextureGenerator::generateDirtyConcreteTexture(uint32_t width, uint32_t height, uint32_t seed) {
    TextureBuffer tex{width, height, std::vector<uint32_t>(width * height)};

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            float n1 = perlinNoise2D(x * 0.02f, y * 0.02f, seed);
            float n2 = perlinNoise2D(x * 0.1f, y * 0.1f, seed + 1);

            uint8_t baseGrey = static_cast<uint8_t>(std::clamp((n1 * 0.7f + n2 * 0.3f) * 180.0f + 40.0f, 0.0f, 255.0f));
            uint8_t rustR = static_cast<uint8_t>(std::min(255.0f, baseGrey * 1.2f));
            uint8_t rustG = static_cast<uint8_t>(baseGrey * 0.7f);
            uint8_t rustB = static_cast<uint8_t>(baseGrey * 0.5f);

            // RGBA uint32
            tex.pixelsRGBA[y * width + x] = (255 << 24) | (rustB << 16) | (rustG << 8) | rustR;
        }
    }
    return tex;
}

TextureBuffer TextureGenerator::generatePlatformTileBloodTexture(uint32_t width, uint32_t height, uint32_t seed) {
    TextureBuffer tex{width, height, std::vector<uint32_t>(width * height)};

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            bool isGrout = (x % 32 == 0) || (y % 32 == 0);

            uint8_t r = isGrout ? 60 : 210;
            uint8_t g = isGrout ? 60 : 210;
            uint8_t b = isGrout ? 60 : 210;

            // Bloodstain overlay procedural splatter
            float bloodNoise = perlinNoise2D(x * 0.05f, y * 0.05f, seed + 99);
            if (bloodNoise > 0.65f) {
                r = 160;
                g = 10;
                b = 10;
            }

            tex.pixelsRGBA[y * width + x] = (255 << 24) | (b << 16) | (g << 8) | r;
        }
    }
    return tex;
}

TextureBuffer TextureGenerator::generatePerlinNoiseGrainTexture(uint32_t width, uint32_t height, uint32_t seed) {
    TextureBuffer tex{width, height, std::vector<uint32_t>(width * height)};

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            float n = perlinNoise2D(x * 0.25f, y * 0.25f, seed);
            uint8_t grain = static_cast<uint8_t>(n * 255.0f);

            tex.pixelsRGBA[y * width + x] = (255 << 24) | (grain << 16) | (grain << 8) | grain;
        }
    }
    return tex;
}

} // namespace the_stations
