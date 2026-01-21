#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Entity;
class World;

class MovementSystem {
public:
    MovementSystem() = default;

    static void update(Entity& entity, const World& world, float dt);

    static void moveEntity(Entity& entity, const World& world, const sf::Vector2f& newPosition);

private:
    static void applyCollisions(Entity& entity, const World& world, float dt);

    static sf::Vector2f tryMove(Entity& entity, const World& world, const sf::Vector2f& newPosition);
};