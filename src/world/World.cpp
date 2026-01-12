//
// Created by angel on 8/01/2026.
//

#include "World.h"
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
    auto chunk = std::make_unique<Chunk>(cx, cy, *this);
    chunks[{cx, cy}] = std::move(chunk);

    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx) {
            auto it = chunks.find({cx + dx, cy + dy});
            if (it != chunks.end())
                it->second->finalizeAutoTiling();
        }
}

const std::unordered_map<
    ChunkCoord,
    std::unique_ptr<Chunk>,
    ChunkCoordHash>&
World::getChunks() const {
    return chunks;
}

const Tile* World::getTileGlobal(int wx, int wy) const {
    int cx = floorDiv(wx, Chunk::SIZE);
    int cy = floorDiv(wy, Chunk::SIZE);

    int tx = wx - cx * Chunk::SIZE;
    int ty = wy - cy * Chunk::SIZE;



    auto it = chunks.find({cx, cy});
    if (it == chunks.end())
        return nullptr;

    return &it->second->getTile(tx, ty);
}