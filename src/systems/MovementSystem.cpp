#include "MovementSystem.h"
#include "entities/Entity.h"
#include "world/World.h"
#include "CollisionSystem.h"
#include "core/Config.h"

void MovementSystem::update(Entity& entity, const World& world, float dt) {
    applyCollisions(entity, world, dt);
}

void MovementSystem::moveEntity(Entity& entity, const World& world, const sf::Vector2f& newPosition) {
    sf::Vector2f finalPosition = tryMove(entity, world, newPosition);
    entity.setPosition(finalPosition);
}

void MovementSystem::applyCollisions(Entity& entity, const World& world, float dt) {
    sf::Vector2f velocity = entity.getVelocity();

    if (velocity.x == 0.0f && velocity.y == 0.0f) {
        return;
    }

    sf::Vector2f currentPos = entity.getPosition();
    sf::Vector2f desiredPosition = currentPos + velocity * dt;

    sf::Vector2f finalPosition = tryMove(entity, world, desiredPosition);
    
    entity.setPosition(finalPosition);
}

sf::Vector2f MovementSystem::tryMove(Entity& entity, const World& world, const sf::Vector2f& newPosition) {
    sf::FloatRect localBounds = entity.getLocalBoundingBox();
    
    sf::FloatRect testBounds(
        newPosition.x + localBounds.left,
        newPosition.y + localBounds.top,
        localBounds.width,
        localBounds.height
    );

    int tileX = static_cast<int>(newPosition.x / TILE_SIZE);
    int tileY = static_cast<int>(newPosition.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBounds, world, tileX, tileY, 2)) {
        return newPosition;
    }

    sf::Vector2f currentPos = entity.getPosition();
    sf::Vector2f finalPosition = currentPos;

    sf::Vector2f testPosX(newPosition.x, currentPos.y);
    sf::FloatRect testBoundsX(
        testPosX.x + localBounds.left,
        testPosX.y + localBounds.top,
        localBounds.width,
        localBounds.height
    );

    int tilePosX = static_cast<int>(testPosX.x / TILE_SIZE);
    int tilePosY = static_cast<int>(testPosX.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBoundsX, world, tilePosX, tilePosY, 2)) {
        finalPosition.x = newPosition.x;
    }

    sf::Vector2f testPosY(currentPos.x, newPosition.y);
    sf::FloatRect testBoundsY(
        testPosY.x + localBounds.left,
        testPosY.y + localBounds.top,
        localBounds.width,
        localBounds.height
    );

    tilePosX = static_cast<int>(testPosY.x / TILE_SIZE);
    tilePosY = static_cast<int>(testPosY.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBoundsY, world, tilePosX, tilePosY, 2)) {
        finalPosition.y = newPosition.y;
    }

    return finalPosition;
}