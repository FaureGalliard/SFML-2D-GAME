#pragma once
#include <cstdint>

enum class WorldObjectType : uint8_t {
    GrassTuft,
    Flower,
    Tree1x3,
    Tree2x3,
    Rock,
    Bush
};

struct WorldObject {
    WorldObjectType type;
    uint8_t variant;
    uint16_t tileX;
    uint16_t tileY;

    WorldObject(WorldObjectType type, uint8_t variant, int x, int y)
        : type(type),
          variant(variant),
          tileX(static_cast<uint8_t>(x)),
          tileY(static_cast<uint8_t>(y)) {}
};

