// SpawnRules.cpp
#include "SpawnRules.h"

static const std::vector<SpawnRule> rules = {
    { WorldObjectType::Flower, { TileType::Grass }, 0.12f },
    { WorldObjectType::Rock,   { TileType::Grass, TileType::Sand }, 0.05f },
    { WorldObjectType::Bush,   { TileType::Grass }, 0.08f }
};

const SpawnRule* getSpawnRule(WorldObjectType type) {
    for (const auto& r : rules)
        if (r.type == type)
            return &r;
    return nullptr;
}

bool canSpawn(WorldObjectType type, TileType tile) {
    const SpawnRule* rule = getSpawnRule(type);
    if (!rule) return false;

    for (TileType t : rule->allowed)
        if (t == tile)
            return true;

    return false;
}
