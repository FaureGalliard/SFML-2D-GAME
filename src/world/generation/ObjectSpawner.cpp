#include "ObjectSpawner.h"
#include "BiomeNoise.h"
#include "world/Chunk.h"
#include "objects/SpawnRules.h"

ObjectSpawner::ObjectSpawner(uint32_t seed, const BiomeNoise& biomeNoise)
    : biomeNoise(biomeNoise)
    , rng(seed)
    , dist(0.0f, 1.0f) {
}

void ObjectSpawner::spawnObjects(Chunk& chunk, int cx, int cy) {
    spawnTrees(chunk, cx, cy);

    spawnDecorations(chunk, cx, cy);
}

void ObjectSpawner::spawnTrees(Chunk& chunk, int cx, int cy) {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {

            if (chunk.occupied[y][x]) continue;

            TileType ground = chunk.tiles[y][x].type;
            if (ground != TileType::Grass) continue;

            int wx = cx * CHUNK_SIZE + x;
            int wy = cy * CHUNK_SIZE + y;

            float forest = biomeNoise.sampleForest(wx, wy);

            if (forest < 0.40f) continue;

            if (biomeNoise.isClearing(wx, wy)) continue;

            float density = (forest - 0.40f) / 0.45f;

            float detail = biomeNoise.sampleDetail(wx, wy);
            density += detail * 0.15f; // Pequeña variación
            density = std::clamp(density, 0.0f, 1.0f);

            float roll = dist(rng);

            if (roll > density * 0.85f) continue;

            int variant = 0;
            if (detail > 0.3f) variant = 1;
            else if (detail < -0.3f) variant = 2;

            chunk.worldObjects.emplace_back(
                WorldObjectType::Tree2x3,
                variant,
                x,
                y
            );
            chunk.occupied[y][x] = true;
        }
    }
}

void ObjectSpawner::spawnDecorations(Chunk& chunk, int cx, int cy) {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {

            if (chunk.occupied[y][x]) continue;

            TileType ground = chunk.tiles[y][x].type;

            int wx = cx * CHUNK_SIZE + x;
            int wy = cy * CHUNK_SIZE + y;

            float roll = dist(rng);

            if (ground == TileType::Grass) {
                float forest = biomeNoise.sampleForest(wx, wy);

                if (forest < 0.5f && roll < 0.02f) {
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Rock,
                        0,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest > 0.55f && biomeNoise.isClearing(wx, wy) && roll < 0.15f) {
                    int flowerVariant = static_cast<int>(roll * 100) % 3;
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Flower,
                        flowerVariant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }
            }

            if (ground == TileType::Sand && roll < 0.08f) {
                chunk.worldObjects.emplace_back(
                    WorldObjectType::Bush,
                    0,
                    x,
                    y
                );
                chunk.occupied[y][x] = true;
            }
        }
    }
}