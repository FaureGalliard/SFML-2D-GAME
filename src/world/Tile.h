#pragma once
#include <cstdint>

enum class TileType{Water,Grass,Sand};

struct Tile{

    TileType type;
    uint8_t variant = 0;
    uint8_t mask = 0;
};
