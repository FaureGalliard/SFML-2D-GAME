#pragma once
#include "world/Tile.h"
#include "Noise.h"
#include "core/Config.h"

class Chunk;

struct TerrainParams {
    static constexpr float TERRAIN_SCALE = 64.0f;
    static constexpr float DETAIL_SCALE = 12.0f;
    static constexpr float TERRAIN_WEIGHT = 0.85f;
    static constexpr float DETAIL_WEIGHT = 0.15f;
    
    static constexpr float WATER_THRESHOLD = 0.14f;
    static constexpr float SAND_THRESHOLD = 0.16f;
    
    static constexpr float VARIANT_SCALE = 8.0f;
    static constexpr int VARIANT_OFFSET = 1000;
};

class TerrainGenerator {
public:
    explicit TerrainGenerator(uint32_t seed);
    
    void generateTerrain(Chunk& chunk, int cx, int cy);
    
private:
    Noise noise;
    
    float sampleNoise(int wx, int wy) const;
    TileType determineTileType(float noiseValue) const;
    uint8_t determineVariant(int wx, int wy, TileType type) const;
};