#pragma once
#include <cstdint>
#include <array>
class Noise {
public:
    explicit Noise(uint32_t seed);
    float simplex(float x, float y) const;

private:
    std::array<int, 512> perm;
};