#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Chunk.h"
#include "core/Config.h"

class TerrainGenerator;
class ObjectSpawner;

struct ChunkCoord {
    int x, y;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

class World {
public:
    explicit World(uint32_t seed);
    ~World();


    void update(int playerTileX, int playerTileY);

    const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>&
    getChunks() const;


    std::vector<Chunk*> getVisibleChunks(const sf::FloatRect& cameraBounds) const;

    const Tile* getTileGlobal(int wx, int wy) const;

    Chunk* getChunk(int cx, int cy);
    const Chunk* getChunk(int cx, int cy) const;

private:
    static constexpr int LOAD_RADIUS = 2;

    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;
    std::unique_ptr<TerrainGenerator> terrainGen;
    std::unique_ptr<ObjectSpawner> objSpawner;

    void loadChunk(int cx, int cy);

    void unloadFarChunks(int centerCx, int centerCy);
};