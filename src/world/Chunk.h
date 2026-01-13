#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include "world/Tile.h"
#include "worldobjects/WorldObject.h"
#include <array>
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
    void finalizeAutoTiling();
    void generateObjects();
    void invalidateObjectMesh();
    void generate();
    uint8_t computeMask(int x, int y) const;
    bool sameType(int x, int y, TileType t) const;

    bool meshBuilt = false;
    bool objectMeshBuilt = false;
    std::vector<WorldObject> worldObjects;
    std::array<std::array<bool, CHUNK_SIZE>, CHUNK_SIZE> occupied{};
    int cx, cy;
    const World& world;
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    sf::VertexArray mesh;
    sf::VertexArray objectMesh;

};