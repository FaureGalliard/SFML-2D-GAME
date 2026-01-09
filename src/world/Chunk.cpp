//
// Created by angel on 8/01/2026.
//

#include "Chunk.h"

Chunk::Chunk(int cx, int cy) {
    generate();
}

void Chunk::generate() {


}

const Tile& Chunk::getTile(int x, int y) const{
    return tiles[x][y];
}