// SpawnRules.h
#pragma once
#include <vector>
#include "world/Tile.h"
#include "WorldObject.h"

struct SpawnRule {
    WorldObjectType type;
    std::vector<TileType> allowed;
    float probability;
};

bool canSpawn(WorldObjectType type, TileType tile);
const SpawnRule* getSpawnRule(WorldObjectType type);
