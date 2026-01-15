#include "ObjectSpawner.h"
#include "world/Chunk.h"
#include "objects/SpawnRules.h"

ObjectSpawner::ObjectSpawner(uint32_t seed) 
    : rng(seed), dist(0.0f, 1.0f) {
}

void ObjectSpawner::spawnObjects(Chunk& chunk, int cx, int cy) {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            if (chunk.occupied[y][x]) {
                continue;
            }
            
            TileType ground = chunk.tiles[y][x].type;
            
            for (const SpawnRule& rule : getAllSpawnRules()) {
                if (!canSpawn(rule.type, ground)) {
                    continue;
                }
                
                float roll = dist(rng);
                if (roll > rule.probability) {
                    continue;
                }
                
                chunk.worldObjects.emplace_back(rule.type, 0, x, y);
                chunk.occupied[y][x] = true;
                break;
            }
        }
    }
}