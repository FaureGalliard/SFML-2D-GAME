#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "core/Config.h"

class Hero {
public:
    Hero();
    Hero(float x, float y);

    void update(float dt);

    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos) { position = pos; }

    int getTileX() const { return static_cast<int>(position.x / TILE_SIZE); }
    int getTileY() const { return static_cast<int>(position.y / TILE_SIZE); }
    sf::Vector2i getTilePosition() const { return {getTileX(), getTileY()}; }

    const sf::Sprite& getSprite() const { return sprite; }

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Sprite sprite;

    float speed = 100.0f;
};