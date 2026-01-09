#pragma once
#include <iostream>
#include <cstdint>

enum class TileType{Water,Grass,Sand};

struct Tile{

    TileType type;
    uint8_t variant = 0;
};
