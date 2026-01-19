#include "BiomeNoise.h"

BiomeNoise::BiomeNoise(uint32_t seed)
    : forestNoise(seed + 12345)
    , clearingNoise(seed + 67890)
    , detailNoise(seed + 11111)
{
}

float BiomeNoise::sampleForest(int wx, int wy) const {

    float n = forestNoise.simplex(
        wx / 96.0f,
        wy / 96.0f
    );

    return (n + 1.0f) * 0.5f;
}

bool BiomeNoise::isClearing(int wx, int wy) const {

    float clearing = clearingNoise.simplex(
        (wx + 1000) / 24.0f,
        (wy + 1000) / 24.0f
    );


    return clearing > 0.3f;
}

float BiomeNoise::sampleDetail(int wx, int wy) const {

    return detailNoise.simplex(
        wx / 8.0f,
        wy / 8.0f
    );
}