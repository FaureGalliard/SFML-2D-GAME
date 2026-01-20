#pragma once
#include <vector>
#include <cstdint>

struct ObjectVariant {
    uint8_t x;  // Columna en el tileset
    uint8_t y;  // Fila en el tileset
};

struct ObjectVisual {
    uint8_t width;   // Ancho en tiles
    uint8_t height;  // Alto en tiles
    std::vector<ObjectVariant> variants;
};