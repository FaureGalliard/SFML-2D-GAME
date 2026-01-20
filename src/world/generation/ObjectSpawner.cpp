#include "ObjectSpawner.h"
#include "BiomeNoise.h"
#include "world/Chunk.h"
#include "objects/ObjectAtlas.h"
#include <vector>

struct SpawnableObject {
    WorldObjectType type;
    float probability;
};

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
                float detailNormalized = (detail + 1.0f) * 0.5f;
                variant = static_cast<uint8_t>(detailNormalized * visual.variants.size()) % visual.variants.size();
            }

            chunk.worldObjects.emplace_back(treeType, variant, x, y);
            chunk.occupied[y][x] = true;
        }
    }
}

WorldObjectType selectRandomObject(const std::vector<SpawnableObject>& objects, float roll) {
    float totalProb = 0.0f;
    for (const auto& obj : objects) {
        totalProb += obj.probability;
    }

    float threshold = roll * totalProb;

    float accumulated = 0.0f;
    for (const auto& obj : objects) {
        accumulated += obj.probability;
        if (threshold <= accumulated) {
            return obj.type;
        }
    }

    return objects[0].type;
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
                if (forest < 0.6f && roll < 0.015f) {
                    std::vector<SpawnableObject> openFieldObjects = {
                        {WorldObjectType::Rock,   2.0f},  // 30% de probabilidad
                        {WorldObjectType::Flower, 3.5f},  // 50% de probabilidad
                        {WorldObjectType::Bush,   1.0f}   // 20% de probabilidad
                    };

                    float objectRoll = dist(rng);
                    WorldObjectType selectedType = selectRandomObject(openFieldObjects, objectRoll);

                    const ObjectVisual& visual = getObjectVisual(selectedType);
                    uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(selectedType, variant, x, y);
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.65f && inClearing && roll < 0.12f) {
                    std::vector<SpawnableObject> clearingObjects = {
                        {WorldObjectType::Flower, 4.0f},
                        {WorldObjectType::Bush,   2.5f},
                        {WorldObjectType::Rock,   4.0f}
                    };

                    float objectRoll = dist(rng);
                    WorldObjectType selectedType = selectRandomObject(clearingObjects, objectRoll);

                    const ObjectVisual& visual = getObjectVisual(selectedType);

                    uint8_t variant = 0;
                    if (selectedType == WorldObjectType::Flower) {
                        float detailNormalized = (detail + 1.0f) * 0.5f;
                        variant = static_cast<uint8_t>(detailNormalized * visual.variants.size()) % visual.variants.size();
                    } else {
                        variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();
                    }

                    chunk.worldObjects.emplace_back(selectedType, variant, x, y);
                    chunk.occupied[y][x] = true;
                    continue;
                }

                if (forest >= 0.6f && forest < 0.7f && roll < 0.03f) {
                    std::vector<SpawnableObject> transitionObjects = {
                        {WorldObjectType::Bush,   3.0f},
                        {WorldObjectType::Flower, 1.0f},
                        {WorldObjectType::Rock,   1.0f}
                    };

                    float objectRoll = dist(rng);
                    WorldObjectType selectedType = selectRandomObject(transitionObjects, objectRoll);

                    const ObjectVisual& visual = getObjectVisual(selectedType);
                    uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                    chunk.worldObjects.emplace_back(selectedType, variant, x, y);
                    chunk.occupied[y][x] = true;
                    continue;
                }
            }

            if (ground == TileType::Sand && roll < 0.025f) {
                std::vector<SpawnableObject> sandObjects = {
                    {WorldObjectType::Rock,   6.0f},

                };

                float objectRoll = dist(rng);
                WorldObjectType selectedType = selectRandomObject(sandObjects, objectRoll);

                const ObjectVisual& visual = getObjectVisual(selectedType);
                uint8_t variant = static_cast<uint8_t>(std::abs(detail) * visual.variants.size()) % visual.variants.size();

                chunk.worldObjects.emplace_back(selectedType, variant, x, y);
                chunk.occupied[y][x] = true;
            }
        }
    }
}