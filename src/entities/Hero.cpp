#include "Hero.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
Hero::Hero() : position(0.0f, 0.0f), velocity(0.0f, 0.0f) {
}

Hero::Hero(float x, float y) : position(x, y), velocity(0.0f, 0.0f) {
}

void Hero::update(float dt) {
    velocity = {0.0f, 0.0f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y = -speed;
        }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocity.y = speed;
        }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -speed;
        }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = speed;
        }

    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length > 0) {
        velocity.x = (velocity.x / length) * speed;
        velocity.y = (velocity.y / length) * speed;
    }

    position += velocity * dt;

    sprite.setPosition(position);
}