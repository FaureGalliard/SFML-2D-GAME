#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <array>
#include "world/Tile.h"
#include "objects/WorldObject.h"
#include "core/Config.h"

class WorldRenderer;
class World;

class Chunk {
public:

    Chunk(int cx, int cy, const World& world);

    bool isMeshBuilt() const { return meshBuilt; }
    bool isObjectMeshBuilt() const { return objectMeshBuilt; }
    void buildMesh(const WorldRenderer& renderer);
    void buildObjectMesh(const WorldRenderer& renderer);
    void invalidateObjectMesh();

    void finalizeAutoTiling();
    uint8_t computeMask(int x, int y) const;
    bool sameType(int x, int y, TileType t) const;

    const World& world;
    int cx, cy;
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    std::vector<WorldObject> worldObjects;
    std::array<std::array<bool, CHUNK_SIZE>, CHUNK_SIZE> occupied{};
    sf::VertexArray mesh;
    sf::VertexArray objectMesh;

private:
    bool meshBuilt = false;
    bool objectMeshBuilt = false;
};