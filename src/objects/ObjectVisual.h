#pragma once
#include <vector>
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>

struct ObjectVariant {
    uint8_t x;  // Columna en el tileset
    uint8_t y;  // Fila en el tileset
};

struct ObjectVisual {
    uint8_t width;   // Ancho en tiles
    uint8_t height;  // Alto en tiles
    std::vector<ObjectVariant> variants;


    sf::FloatRect hitbox;
    bool hasCollision;

    ObjectVisual()
        : width(1), height(1), hasCollision(false) {}

    ObjectVisual(uint8_t w, uint8_t h, std::vector<ObjectVariant> vars,
                 bool collision = false, sf::FloatRect customHitbox = sf::FloatRect())
        : width(w), height(h), variants(vars), hasCollision(collision) {

        if (customHitbox.width > 0 && customHitbox.height > 0) {
            hitbox = customHitbox;
        } else if (collision) {
            hitbox = sf::FloatRect(
                -float(w * 8),
                -float(h * 4),
                float(w * 16),
                float(h * 8)
            );
        }
    }
};
