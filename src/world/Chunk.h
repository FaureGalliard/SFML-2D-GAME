#pragma once
#include "world/Tile.h"

class Chunk {

    public:
        static constexpr int SIZE = 16;
        Chunk(int cx,int cy);
        const Tile& getTile(int x, int y) const;

    private:
        int cx,cy;
        Tile tiles[SIZE][SIZE];
        void generate();

};
