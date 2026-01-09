//
// Created by angel on 8/01/2026.
//

#include "Chunk.h"
#include "Noise.h"
#include <cstdint>


static constexpr int WORLD_SEED = 1339;
Chunk::Chunk(int cx, int cy) {
    generate();
    finalizeAutoTiling();
}

void Chunk::generate() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {

            int wx = cx * SIZE + x;
            int wy = cy * SIZE + y;

            float terrain = (Noise::value(wx / 12, wy / 12, WORLD_SEED) + 1.f) * 0.5f;
            float detail  = (Noise::value(wx, wy, WORLD_SEED + 42) + 1.f) * 0.5f;

            float n = terrain * 0.8f + detail * 0.2f;

            Tile& tile = tiles[y][x];

            if (n < 0.8f) {
                tile.type = TileType::Grass;

                tile.variant = (Noise::value(wx + 999, wy + 999, WORLD_SEED) > 0)
                               ? 1
                               : 0;
            }
            else if (n < 0.95f) {
                tile.type = TileType::Sand;
                tile.variant = 0;
            }
            else {
                tile.type = TileType::Water;
                tile.variant = 0;
            }
        }
    }

}

const Tile& Chunk::getTile(int x, int y) const{
    return tiles[y][x];
}

bool Chunk::sameType(int x, int y, TileType t) const {

        if (x<0 || y < 0 || x>= SIZE || y>= SIZE) return false;
        return tiles[y][x].type == t;

}

uint8_t Chunk::computeMask(int x,int y) const {
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