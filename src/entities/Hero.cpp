//
// Created by angel on 9/01/2026.
//

#include "Hero.h"
#include <SFML/Window/Keyboard.hpp>

constexpr int TILE_SIZE = 16;

Hero::Hero() {
    position = {0.f, 0.f};
}

void Hero::update(float dt) {
    sf::Vector2f dir{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1;

    position += dir * speed * dt;
}

int Hero::getTileX() const {
    return static_cast<int>(position.x) / TILE_SIZE;
}

int Hero::getTileY() const {
    return static_cast<int>(position.y) / TILE_SIZE;
}