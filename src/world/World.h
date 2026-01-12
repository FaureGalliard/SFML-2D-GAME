#pragma once
#include <unordered_map>
#include <memory>
#include "Chunk.h"

struct ChunkCoord {
    int x,y;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const {
        return std::hash<int>()(c.x)^(std::hash<int>()(c.y)<<1);
    }
};

class World {
public:
    World();
    void update(int playerTileX,int playerTileY);
    const std::unordered_map<ChunkCoord,
    std::unique_ptr<Chunk>,ChunkCoordHash>&
    getChunks() const;
private:
    static constexpr int LOAD_RADIUS = 2;
    std::unordered_map<ChunkCoord,std::unique_ptr<Chunk>,ChunkCoordHash> chunks;
    void loadChunk(int cx,int cy);
    void unloadFarChunks(int centerCx,int centerCy);
};