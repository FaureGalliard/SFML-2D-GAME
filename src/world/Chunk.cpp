//
// Created by angel on 8/01/2026.
//

#include "Chunk.h"
#include "Noise.h"
#include <cstdlib>

static constexpr int WORLD_SEED = 1342;
static Noise noise(WORLD_SEED);

Chunk::Chunk(int cx, int cy)
    : cx(cx), cy(cy)
{
    generate();
    finalizeAutoTiling();
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

            if (n < 0.18f) {
                tile.type = TileType::Water;
                tile.variant = 0;
            }
            else if (n < 0.42f) {
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


bool Chunk::sameType(int x, int y, TileType t) const {
    if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
        return false;
    return tiles[y][x].type == t;
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
