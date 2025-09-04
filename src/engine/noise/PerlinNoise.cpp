#include "../../../vendor/glad/include/glad/glad.h"
#include <ctime>
#include <glm/glm.hpp>
#include "PerlinNoise.h"
#include <cmath>

// const unsigned int SEED = 1337u;
const unsigned int SEED = static_cast<unsigned int>(time(nullptr));

float PerlinNoise::Fade(float t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::Interpolate(float a, float b, float t) const {
    return a + t * (b - a);
}

float PerlinNoise::Smoothstep(float t) const {
    return t * t * (3 - 2 * t);
}

static glm::vec2 RandomGradient(int ix, int iy) {
    unsigned int hash = ix * 374761393u + iy * 668265263u + SEED;

    hash = (hash ^ (hash >> 13u)) * 1274126177u;
    hash = (hash ^ (hash >> 16u));

    float angle = (hash % 360) * 3.14159265359f / 180.0f; 

    return glm::vec2(cos(angle), sin(angle)); 
}

float PerlinNoise::DotGridGradient(int ix, int iy, float x, float y) const {
    glm::vec2 gradient = RandomGradient(ix, iy); 
    glm::vec2 distance(x - ix, y - iy); 

    return glm::dot(gradient, distance);
}

float PerlinNoise::SampleNoise(float x, float y) const {
    int x0 = static_cast<int>(floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(floor(y));
    int y1 = y0 + 1;

    float sx = Smoothstep(x - x0);
    float sy = Smoothstep(y - y0);

    float n0 = DotGridGradient(x0, y0, x, y);
    float n1 = DotGridGradient(x1, y0, x, y);
    float ix0 = Interpolate(n0, n1, sx); 

    n0 = DotGridGradient(x0, y1, x, y);
    n1 = DotGridGradient(x1, y1, x, y);
    float ix1 = Interpolate(n0, n1, sx); 

    return Interpolate(ix0, ix1, sy); 
}

std::vector<unsigned char> PerlinNoise::GenerateNoise(int width, int height, float scale) const {
    std::vector<unsigned char> data(width * height * 3); // RGB format

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Convert to texture size.
            float xf = x / static_cast<float>(width) * scale;
            float yf = y / static_cast<float>(height) * scale;

            // Normalize Perlin noise to [0, 1]
            float noise = SampleNoise(xf, yf) * 0.5f + 0.5f;

            // Map to [0, 255]
            unsigned char value = static_cast<unsigned char>(noise * 255);

            // RGB format
            int index = (y * width + x) * 3;
            // data[index] = (value*23) % 255;     // Red
            data[index] = value;     // Red
            data[index + 1] = value; // Green
            data[index + 2] = value; // Blue
        }
    }

    return data;
}
