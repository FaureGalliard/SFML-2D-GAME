#pragma once
#include <cstdint>
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>
#include "worldobjects/WorldObject.h"

struct ObjectVisual {
    uint8_t startX;
    uint8_t startY;
    uint8_t width;
    uint8_t height;
};