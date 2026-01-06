#pragma once
#include <SFML/Graphics.hpp>

class Chunk {

private:
    int cx, cy;
    uint32_t seed;
    Tile tiles[SIZE][SIZE];

public:
    static constexpr int SIZE = 16;

    Chunk(int cx, int cy, uint32_t seed);

    void generate();
    Tile getTile(int x, int y) const;

    void generate() {
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int wx = cx * SIZE + x;
                int wy = cy * SIZE + y;

                float n = noise(wx, wy, seed);

                if (n < 0.3f)
                    tiles[y][x].type = GRASS;
                else if (n < 0.6f)
                    tiles[y][x].type = FOREST;
                else
                    tiles[y][x].type = RUINS;
            }
        }
    }

};