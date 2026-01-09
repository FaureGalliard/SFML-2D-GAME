#pragma once
#include <cstdint>

class Noise {
public:
    explicit Noise(int seed);
    float simplex(float x, float y) const;

private:
    int seed;
};