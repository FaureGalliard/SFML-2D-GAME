//
// Created by angel on 8/01/2026.
//

#include "Chunk.h"
#include "Noise.h"
#include <cstdint>


static constexpr int WORLD_SEED = 1337;
Chunk::Chunk(int cx, int cy) {
    generate();
}

void Chunk::generate() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {

            // Coordenadas mundiales
            int wx = cx * SIZE + x;
            int wy = cy * SIZE + y;

            // Noise normalizado [0,1]
            float n = (Noise::value(wx, wy, WORLD_SEED) + 1.f) * 0.5f;

            Tile& tile = tiles[y][x];

            if (n < 0.65f) {
                tile.type = TileType::Grass;

                // VARIANTE DETERMINISTA
                tile.variant = (Noise::value(wx + 999, wy + 999, WORLD_SEED) > 0)
                               ? 1
                               : 0;
            }
            else if (n < 0.8f) {
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