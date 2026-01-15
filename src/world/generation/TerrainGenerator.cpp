#include "TerrainGenerator.h"
#include "world/Chunk.h"

TerrainGenerator::TerrainGenerator(uint32_t seed) 
    : noise(seed) {
}

void TerrainGenerator::generateTerrain(Chunk& chunk, int cx, int cy) {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            int wx = cx * CHUNK_SIZE + x;
            int wy = cy * CHUNK_SIZE + y;
            
            float noiseValue = sampleNoise(wx, wy);
            
            Tile& tile = chunk.tiles[y][x];
            tile.type = determineTileType(noiseValue);
            tile.variant = determineVariant(wx, wy, tile.type);
        }
    }
}

float TerrainGenerator::sampleNoise(int wx, int wy) const {
    // Terrain base
    float terrain = noise.simplex(wx / TerrainParams::TERRAIN_SCALE, 
                                   wy / TerrainParams::TERRAIN_SCALE);
    terrain = (terrain + 1.0f) * 0.5f;
    
    // Detail layer
    float detail = noise.simplex(wx / TerrainParams::DETAIL_SCALE, 
                                  wy / TerrainParams::DETAIL_SCALE);
    detail = (detail + 1.0f) * 0.5f;
    
    // Combine
    return terrain * TerrainParams::TERRAIN_WEIGHT + 
           detail * TerrainParams::DETAIL_WEIGHT;
}

TileType TerrainGenerator::determineTileType(float noiseValue) const {
    if (noiseValue < TerrainParams::WATER_THRESHOLD) {
        return TileType::Water;
    }
    else if (noiseValue < TerrainParams::SAND_THRESHOLD) {
        return TileType::Sand;
    }
    else {
        return TileType::Grass;
    }
}

uint8_t TerrainGenerator::determineVariant(int wx, int wy, TileType type) const {
    if (type != TileType::Grass) {
        return 0;
    }
    
    float variantNoise = noise.simplex(
        (wx + TerrainParams::VARIANT_OFFSET) / TerrainParams::VARIANT_SCALE,
        (wy + TerrainParams::VARIANT_OFFSET) / TerrainParams::VARIANT_SCALE
    );
    
    return variantNoise > 0 ? 1 : 0;
}