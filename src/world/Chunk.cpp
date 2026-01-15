#include "Chunk.h"
#include "World.h"
#include "render/WorldRenderer.h"

Chunk::Chunk(int cx, int cy, const World& world)
    : world(world),
      cx(cx),
      cy(cy),
      mesh(sf::Quads),
      objectMesh(sf::Quads)
{
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            tiles[y][x] = {TileType::Grass, 0, 0};
        }
    }
}

bool Chunk::sameType(int x, int y, TileType t) const {
    int wx = cx * CHUNK_SIZE + x;
    int wy = cy * CHUNK_SIZE + y;
    const Tile* tile = world.getTileGlobal(wx, wy);
    return tile && tile->type == t;
}

uint8_t Chunk::computeMask(int x, int y) const {
    TileType t = tiles[y][x].type;
    uint8_t mask = 0;
    if (sameType(x, y - 1, t)) mask |= 1; // arriba
    if (sameType(x + 1, y, t)) mask |= 2; // derecha
    if (sameType(x, y + 1, t)) mask |= 4; // abajo
    if (sameType(x - 1, y, t)) mask |= 8; // izquierda
    return mask;
}

void Chunk::finalizeAutoTiling() {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            tiles[y][x].mask = computeMask(x, y);
        }
    }
}

void Chunk::buildMesh(const WorldRenderer& renderer) {
    mesh.clear();
    mesh.setPrimitiveType(sf::Quads);
    mesh.resize(CHUNK_SIZE * CHUNK_SIZE * 4);

    int i = 0;
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            const Tile& tile = tiles[y][x];
            sf::IntRect tex = renderer.pickTileRect(tile);

            float px = (cx * CHUNK_SIZE + x) * TILE_SIZE;
            float py = (cy * CHUNK_SIZE + y) * TILE_SIZE;

            sf::Vertex* q = &mesh[i * 4];
            q[0].position = {px, py};
            q[1].position = {px + TILE_SIZE, py};
            q[2].position = {px + TILE_SIZE, py + TILE_SIZE};
            q[3].position = {px, py + TILE_SIZE};

            q[0].texCoords = {float(tex.left), float(tex.top)};
            q[1].texCoords = {float(tex.left + tex.width), float(tex.top)};
            q[2].texCoords = {float(tex.left + tex.width), float(tex.top + tex.height)};
            q[3].texCoords = {float(tex.left), float(tex.top + tex.height)};

            ++i;
        }
    }
    meshBuilt = true;
}

void Chunk::buildObjectMesh(const WorldRenderer& renderer) {
    objectMesh.clear();
    objectMesh.setPrimitiveType(sf::Quads);
    objectMesh.resize(worldObjects.size() * 4);

    size_t vertexIndex = 0;
    for (const WorldObject& obj : worldObjects) {
        sf::IntRect tex = renderer.pickObjectRect(obj);

        float wx = (cx * CHUNK_SIZE + obj.tileX) * TILE_SIZE;
        float wy = (cy * CHUNK_SIZE + obj.tileY) * TILE_SIZE;
        float px = wx + TILE_SIZE * 0.5f;
        float py = wy + TILE_SIZE;

        sf::Vertex* q = &objectMesh[vertexIndex];
        q[0].position = {px - tex.width * 0.5f, py - tex.height};
        q[1].position = {px + tex.width * 0.5f, py - tex.height};
        q[2].position = {px + tex.width * 0.5f, py};
        q[3].position = {px - tex.width * 0.5f, py};

        q[0].texCoords = {float(tex.left), float(tex.top)};
        q[1].texCoords = {float(tex.left + tex.width), float(tex.top)};
        q[2].texCoords = {float(tex.left + tex.width), float(tex.top + tex.height)};
        q[3].texCoords = {float(tex.left), float(tex.top + tex.height)};

        vertexIndex += 4;
    }
    objectMeshBuilt = true;
}

void Chunk::invalidateObjectMesh() {
    objectMeshBuilt = false;
}