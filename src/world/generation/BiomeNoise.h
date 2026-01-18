#pragma once
#include "Noise.h"
#include <cstdint>


class BiomeNoise {
public:
    explicit BiomeNoise(uint32_t seed);


    float sampleForest(int wx, int wy) const;

    bool isClearing(int wx, int wy) const;


    float sampleDetail(int wx, int wy) const;



private:
    Noise forestNoise;
    Noise clearingNoise;
    Noise detailNoise;
};