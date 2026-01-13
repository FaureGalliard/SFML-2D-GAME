#include "Chunk.h"
#include "Noise.h"
#include "render/WorldRenderer.h"
#include "world/World.h"
#include "worldobjects/SpawnRules.h"
#include <random>

static std::mt19937 rng(12345);
static std::uniform_real_distribution<float> dist(0.f, 1.f);
static Noise noise(WORLD_SEED);

Chunk::Chunk(int cx, int cy, const World& world)
    : world(world),
      cx(cx),
      cy(cy),
      mesh(sf::Quads),
      objectMesh(sf::Quads)
{
    generate();
    finalizeAutoTiling();
    generateObjects();
}

void Chunk::generate() {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            int wx = cx * CHUNK_SIZE + x;
            int wy = cy * CHUNK_SIZE + y;
            float terrain = noise.simplex(wx / 64.f, wy / 64.f);
            terrain = (terrain + 1.f) * 0.5f;
            float detail = noise.simplex(wx / 12.f, wy / 12.f);
            detail = (detail + 1.f) * 0.5f;
            float n = terrain * 0.85f + detail * 0.15f;
            Tile& tile = tiles[y][x];
            if (n < 0.25f) {
                tile.type = TileType::Water;
                tile.variant = 0;
            }
            else if (n < 0.32f) {
                tile.type = TileType::Sand;
                tile.variant = 0;
            }
            else {
                tile.type = TileType::Grass;
                tile.variant =
                    noise.simplex((wx + 1000) / 8.f, (wy + 1000) / 8.f) > 0
                    ? 1
                    : 0;
            }
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
    for (int y = 0; y < CHUNK_SIZE; ++y)
        for (int x = 0; x < CHUNK_SIZE; ++x)
            tiles[y][x].mask = computeMask(x, y);
}

void Chunk::generateObjects() {
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            TileType ground = tiles[y][x].type;
            if (occupied[y][x])
                continue;

            for (const SpawnRule& rule : getAllSpawnRules()) {
                if (!canSpawn(rule.type, ground))
                    continue;
                float r = dist(rng);
                if (r > rule.probability)
                    continue;

                worldObjects.emplace_back(
                    rule.type,
                    0,
                    x, y
                 );
                occupied[y][x] = true;
                break;
            }
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
    for (const WorldObject& obj : worldObjects) {
        sf::IntRect tex = renderer.pickObjectRect(obj);
        float wx = (cx * CHUNK_SIZE + obj.tileX) * TILE_SIZE;
        float wy = (cy * CHUNK_SIZE + obj.tileY) * TILE_SIZE;
        float px = wx + TILE_SIZE * 0.5f;
        float py = wy + TILE_SIZE;
        sf::Vertex quad[4];
        quad[0].position = { px - tex.width * 0.5f, py - tex.height };
        quad[1].position = { px + tex.width * 0.5f, py - tex.height };
        quad[2].position = { px + tex.width * 0.5f, py };
        quad[3].position = { px - tex.width * 0.5f, py };
        quad[0].texCoords = { (float)tex.left, (float)tex.top };
        quad[1].texCoords = { (float)(tex.left + tex.width),(float)tex.top };
        quad[2].texCoords = { (float)(tex.left + tex.width),(float)(tex.top + tex.height) };
        quad[3].texCoords = { (float)tex.left, (float)(tex.top + tex.height) };
        for (int i = 0; i < 4; ++i)
            objectMesh.append(quad[i]);
    }
        objectMeshBuilt = true;
}

void Chunk::invalidateObjectMesh() {
    objectMeshBuilt = false;
}