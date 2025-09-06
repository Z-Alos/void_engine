#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>

class PerlinNoise {
public:
    std::vector<unsigned char> GenerateNoise(int width, int height, float scale, float time) const;
    
private:
    float Fade(float t) const;
    float Interpolate(float a, float b, float t) const;
    float Smoothstep(float t) const;
    float DotGridGradient(int ix, int iy, int iz, float x, float y, float z) const;
    float SampleNoise(float x, float y, float time) const;
};

#endif 
