#include "ObjectSpawner.h"
#include "BiomeNoise.h"
#include "world/Chunk.h"
#include "objects/ObjectAtlas.h"

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

            if (forest < 0.65f) continue;

            if (biomeNoise.isClearing(wx, wy)) continue;

            float density = (forest - 0.65f) / 0.35f;

            float detail = biomeNoise.sampleDetail(wx, wy);
            density += detail * 0.15f;
            density = std::clamp(density, 0.0f, 1.0f);

            float roll = dist(rng);

            if (roll > density * 0.75f) continue;

            WorldObjectType treeType = (roll < 0.7f) ? WorldObjectType::Tree2x3 : WorldObjectType::Tree1x3;

            const ObjectVisual& visual = getObjectVisual(treeType);

            uint8_t variant = 0;
            if (visual.variants.size() > 1) {
                float detailNormalized = (detail + 1.0f) * 0.5f; // [-1,1] → [0,1]
                variant = static_cast<uint8_t>(detailNormalized * visual.variants.size()) % visual.variants.size();
            }

            chunk.worldObjects.emplace_back(
                treeType,
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
            float detail = biomeNoise.sampleDetail(wx, wy);

            if (ground == TileType::Grass) {

                if (forest < 0.6f && roll < 0.005f) {
                    const ObjectVisual& visual = getObjectVisual(WorldObjectType::Rock);
                    uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Rock,
                        variant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;


                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.02f) {
                    const ObjectVisual& visual = getObjectVisual(WorldObjectType::Rock);
                    uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Rock,
                        variant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.03f) {
                    const ObjectVisual& visual = getObjectVisual(WorldObjectType::Bush);
                    uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Bush,
                        variant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.08f) {
                    const ObjectVisual& visual = getObjectVisual(WorldObjectType::Flower);

                    float detailNormalized = (detail + 1.0f) * 0.5f; // [-1,1] → [0,1]
                    uint8_t variant = static_cast<uint8_t>(detailNormalized * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(
                        WorldObjectType::Flower,
                        variant,
                        x,
                        y
                    );
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.6f && forest < 0.7f && roll < 0.02f) {
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

            if (ground == TileType::Sand && roll < 0.02f) {
                chunk.worldObjects.emplace_back(
                    WorldObjectType::Rock,
                    0,
                    x,
                    y
                );
                chunk.occupied[y][x] = true;
            }
        }
    }
}