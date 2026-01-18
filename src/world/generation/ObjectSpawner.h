#pragma once
#include <random>

class Chunk;
class BiomeNoise;

class ObjectSpawner {
public:
    explicit ObjectSpawner(uint32_t seed, const BiomeNoise& biomeNoise);

    void spawnObjects(Chunk& chunk, int cx, int cy);

private:
    const BiomeNoise& biomeNoise;

    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    void spawnTrees(Chunk& chunk, int cx, int cy);

    void spawnDecorations(Chunk& chunk, int cx, int cy);
};