#pragma once
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>

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
    uint16_t tileX;   // Coordenada LOCAL al chunk (0-31)
    uint16_t tileY;   // Coordenada LOCAL al chunk (0-31)

    WorldObject(WorldObjectType type, uint8_t variant, int x, int y)
        : type(type),
          variant(variant),
          tileX(static_cast<uint16_t>(x)),
          tileY(static_cast<uint16_t>(y)) {}


    sf::Vector2f getWorldPosition(int chunkX, int chunkY) const;

    sf::FloatRect getCollisionBounds(int chunkX, int chunkY) const;

    bool hasCollision() const;
};