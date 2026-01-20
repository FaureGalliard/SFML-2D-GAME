#include "Entity.h"
#include "world/World.h"
#include "systems/CollisionSystem.h"

Entity::Entity()
    : position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , state(EntityState::Idle)
    , boundingBox(-6.0f, -40.0f, 12.0f, 12.0f)
    , health(100)
    , maxHealth(100)
    , facingLeft(false)
{
}

Entity::Entity(float x, float y)
    : position(x, y)
    , velocity(0.0f, 0.0f)
    , state(EntityState::Idle)
    , boundingBox(-6.0f, -40.0f, 12.0f, 12.0f)
    , health(100)
    , maxHealth(100)
    , facingLeft(false)
{
}
void Entity::setState(EntityState newState) {
    state = newState;
}

sf::FloatRect Entity::getBounds() const {
    return sf::FloatRect(
        position.x + boundingBox.left,
        position.y + boundingBox.top,
        boundingBox.width,
        boundingBox.height
    );
}

void Entity::applyCollisions(const World& world, float dt) {
    if (velocity.x == 0.0f && velocity.y == 0.0f) {
        return;
    }

    sf::Vector2f desiredPosition = position + velocity * dt;

    position = tryMove(world, desiredPosition);
}

sf::Vector2f Entity::tryMove(const World& world, const sf::Vector2f& newPosition) {
    sf::FloatRect testBounds(
        newPosition.x + boundingBox.left,
        newPosition.y + boundingBox.top,
        boundingBox.width,
        boundingBox.height
    );

    int tileX = static_cast<int>(newPosition.x / TILE_SIZE);
    int tileY = static_cast<int>(newPosition.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBounds, world, tileX, tileY, 2)) {
        return newPosition;
    }

    sf::Vector2f finalPosition = position;

    sf::Vector2f testPosX(newPosition.x, position.y);
    sf::FloatRect testBoundsX(
        testPosX.x + boundingBox.left,
        testPosX.y + boundingBox.top,
        boundingBox.width,
        boundingBox.height
    );

    int tilePosX = static_cast<int>(testPosX.x / TILE_SIZE);
    int tilePosY = static_cast<int>(testPosX.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBoundsX, world, tilePosX, tilePosY, 2)) {
        finalPosition.x = newPosition.x;
    }

    sf::Vector2f testPosY(position.x, newPosition.y);
    sf::FloatRect testBoundsY(
        testPosY.x + boundingBox.left,
        testPosY.y + boundingBox.top,
        boundingBox.width,
        boundingBox.height
    );

    tilePosX = static_cast<int>(testPosY.x / TILE_SIZE);
    tilePosY = static_cast<int>(testPosY.y / TILE_SIZE);

    if (!CollisionSystem::checkWorldObjectCollision(testBoundsY, world, tilePosX, tilePosY, 2)) {
        finalPosition.y = newPosition.y;
    }

    return finalPosition;
}