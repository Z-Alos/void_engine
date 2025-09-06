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

static glm::vec3 RandomGradient(int ix, int iy, int iz) {
    unsigned int hash = ix * 73856093u ^ iy * 19349663u ^ iz * 83492791u ^ SEED;
    hash = (hash ^ (hash >> 13u)) * 1274126177u;
    hash = hash ^ (hash >> 16u);

    float theta = (hash % 360) * 3.14159265359f / 180.0f;
    float phi = ((hash >> 8) % 360) * 3.14159265359f / 180.0f;

    float sinTheta = sin(theta);
    return glm::vec3(sinTheta * cos(phi), sinTheta * sin(phi), cos(theta));
}

float PerlinNoise::DotGridGradient(int ix, int iy, int iz, float x, float y, float z) const {
    glm::vec3 gradient = RandomGradient(ix, iy, iz); 
    glm::vec3 distance(x - ix, y - iy, z - iz); 

    return glm::dot(gradient, distance);
}

float PerlinNoise::SampleNoise(float x, float y, float time) const {
    int x0 = static_cast<int>(floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(floor(y));
    int y1 = y0 + 1;
    int t0 = static_cast<int>(floor(time));
    int t1 = t0 + 1;
    
    float sx = Smoothstep(x - x0);
    float sy = Smoothstep(y - y0);
    float st = Smoothstep(time - t0);
    
    // sample 8 corners of the cube
    float n000 = DotGridGradient(x0, y0, t0, x, y, time);
    float n100 = DotGridGradient(x1, y0, t0, x, y, time);
    float n010 = DotGridGradient(x0, y1, t0, x, y, time);
    float n110 = DotGridGradient(x1, y1, t0, x, y, time);
    float n001 = DotGridGradient(x0, y0, t1, x, y, time);
    float n101 = DotGridGradient(x1, y0, t1, x, y, time);
    float n011 = DotGridGradient(x0, y1, t1, x, y, time);
    float n111 = DotGridGradient(x1, y1, t1, x, y, time);
    
    // interpolate along x
    float ix00 = Interpolate(n000, n100, sx);
    float ix10 = Interpolate(n010, n110, sx);
    float ix01 = Interpolate(n001, n101, sx);
    float ix11 = Interpolate(n011, n111, sx);
    
    // interpolate along y
    float ixy0 = Interpolate(ix00, ix10, sy);
    float ixy1 = Interpolate(ix01, ix11, sy);
    
    // interpolate along time
    return Interpolate(ixy0, ixy1, st);
}

std::vector<unsigned char> PerlinNoise::GenerateNoise(int width, int height, float scale, float time) const {
    std::vector<unsigned char> data(width * height * 3); // RGB format
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float xf = x / static_cast<float>(width) * scale;
            float yf = y / static_cast<float>(height) * scale;
            
            // Multi-octave noise
            float noise = 0.0f;
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float totalAmplitude = 0.0f;
            int octaves = 4; // More octaves = more detail
            for (int i = 0; i < octaves; ++i) {
                noise += SampleNoise(xf * frequency, yf * frequency, time * 0.5f * frequency) * amplitude;
                totalAmplitude += amplitude;
                amplitude *= 0.5f; // Persistence
                frequency *= 2.0f; // Lacunarity
            }
            noise = noise / totalAmplitude * 0.5f + 0.5f; // Normalize to [0,1]
            
            unsigned char value = static_cast<unsigned char>(noise * 255);
            
            int index = (y * width + x) * 3;
            data[index] = (value*23) % 255;     // Red
            // data[index] = value;     // Red
            data[index + 1] = value; // Green
            data[index + 2] = value; // Blue
        }
    }
    
    return data;
}
