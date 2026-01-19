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
    // Primero spawnear árboles (bosques)
    spawnTrees(chunk, cx, cy);

    // Luego decoraciones (piedras, flores, etc.)
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


            if (forest < 0.65f) continue;

            if (biomeNoise.isClearing(wx, wy)) continue;


            float density = (forest - 0.65f) / 0.35f;

            float detail = biomeNoise.sampleDetail(wx, wy);
            density += detail * 0.15f; // Pequeña variación
            density = std::clamp(density, 0.0f, 1.0f);

            float roll = dist(rng);


            if (roll > density * 0.75f) continue;

            int variant = 0;
            if (detail > 0.3f) variant = 1;
            else if (detail < -0.3f) variant = 2;

            chunk.worldObjects.emplace_back(
                WorldObjectType::Tree1x3,
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
            float forest = biomeNoise.sampleForest(wx, wy);
            bool inClearing = biomeNoise.isClearing(wx, wy);

            if (ground == TileType::Grass) {

                if (forest < 0.6f && roll < 0.005f) {
                    int rockVariant = static_cast<int>(roll * 100) % 2;
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Rock,
                        rockVariant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.02f) {
                    int rockVariant = static_cast<int>(roll * 100) % 2;
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Rock,
                        rockVariant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.03f) {
                    int bushVariant = static_cast<int>(roll * 100) % 2;
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Bush,
                        bushVariant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.08f) {
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

                if (forest >= 0.6f && forest < 0.7f && roll < 0.02f) { // Era 0.1 → ahora 0.02 (5× menos)
                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Bush,
                        0,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }
            }

            if (ground == TileType::Sand && roll < 0.02f) { // Era 0.08 → ahora 0.02 (4× menos)
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