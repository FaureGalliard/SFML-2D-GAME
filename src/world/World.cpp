//
// Created by angel on 8/01/2026.
//

#include "World.h"
#include <cmath>
#include <unordered_set>

static int floorDiv(int a, int b) {
    int r = a / b;
    if ((a ^ b) < 0 && a % b) --r;
    return r;
}


World::World(){}

void World::update(int playerTileX,int playerTileY) {

    int centerCx = floorDiv(playerTileX, Chunk::SIZE);
    int centerCy = floorDiv(playerTileY, Chunk::SIZE);

    std::unordered_set<ChunkCoord, ChunkCoordHash> needed;

    for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
        for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
            needed.insert({
                centerCx + dx,
                centerCy + dy
            });
        }
    }

    for (const auto& coord : needed) {
        if (chunks.find(coord) == chunks.end()) {
            loadChunk(coord.x, coord.y);
        }
    }

    for (auto it = chunks.begin(); it != chunks.end();) {
        if (needed.find(it->first) == needed.end()) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void World::loadChunk(int cx, int cy) {
    ChunkCoord key{cx, cy};
    chunks.emplace(key, std::make_unique<Chunk>(cx, cy));
}


const std::unordered_map<
    ChunkCoord,
    std::unique_ptr<Chunk>,
    ChunkCoordHash>&
World::getChunks() const {
    return chunks;
}