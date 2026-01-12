
#include "Chunk.h"
#include "Noise.h"
#include "render/WorldRenderer.h"
#include "world/World.h"
#include "worldobjects/SpawnRules.h"
#include "core/Config.h"
#include <random>

static std::mt19937 rng(12345);
static std::uniform_real_distribution<float> dist(0.f, 1.f);

static constexpr int WORLD_SEED = 1341;
static Noise noise(WORLD_SEED);

Chunk::Chunk(int cx, int cy, const World& world)
    : world(world), cx(cx), cy(cy), mesh(sf::Quads)
{
    generate();
    finalizeAutoTiling();
    generateObjects();
}

void Chunk::generate() {

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {

            int wx = cx * SIZE + x;
            int wy = cy * SIZE + y;

            float terrain = noise.simplex(wx / 64.f, wy / 64.f);
            terrain = (terrain + 1.f) * 0.5f;

            float detail = noise.simplex(wx / 12.f, wy / 12.f);
            detail = (detail + 1.f) * 0.5f;

            float n = terrain * 0.85f + detail * 0.15f;

            Tile& tile = tiles[y][x];

            if (n < 0.23f) {
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



const Tile& Chunk::getTile(int x, int y) const {
    return tiles[y][x];
}

void Chunk::buildMesh(const WorldRenderer& renderer) {
    mesh.clear();
    mesh.setPrimitiveType(sf::Quads);
    mesh.resize(SIZE * SIZE * 4);

    int i = 0;

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {

            const Tile& tile = tiles[y][x];
            sf::IntRect tex = renderer.pickTileRect(tile);

            float px = (cx * SIZE + x) * TILE_SIZE;
            float py = (cy * SIZE + y) * TILE_SIZE;

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

bool Chunk::sameType(int x, int y, TileType t) const {
    int wx = cx * SIZE + x;
    int wy = cy * SIZE + y;

    const Tile* tile = world.getTileGlobal(wx, wy);
    return tile && tile->type == t;
}

const sf::VertexArray& Chunk::getMesh() const {
    return mesh;
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

    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            tiles[y][x].mask = computeMask(x, y);
}

int Chunk::getCX() const {
    return cx;
}

int Chunk::getCY() const {
    return cy;
}


void Chunk::generateObjects() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {

            TileType ground = tiles[y][x].type;


            if (occupied[y][x])
                continue;

            float densityNoise = noise.simplex(
                (cx * SIZE + x + 10000) / 8.f,
                (cy * SIZE + y + 10000) / 8.f
            );
            densityNoise = (densityNoise + 1.f) * 0.5f;

            for (const SpawnRule& rule : getAllSpawnRules()) {

                if (!canSpawn(rule.type, ground))
                    continue;

                float r = dist(rng);
                if (r > rule.probability)
                    continue;

                worldObjects.emplace_back(
                    rule.type,
                    uint8_t(r * 4),
                    x, y
                );

                occupied[y][x] = true;
                break;
            }
        }
    }
}


