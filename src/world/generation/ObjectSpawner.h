#pragma once
#include <random>

class Chunk;

class ObjectSpawner {
public:
    explicit ObjectSpawner(uint32_t seed);
    
    void spawnObjects(Chunk& chunk, int cx, int cy);
    
private:
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
};