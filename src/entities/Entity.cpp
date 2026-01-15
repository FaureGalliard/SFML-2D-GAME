#include "Entity.h"

Entity::Entity()
    : position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , state(EntityState::Idle)
    , boundingBox(0.0f, 0.0f, 32.0f, 32.0f)
    , health(100)
    , maxHealth(100)
    , facingLeft(false)
{
}

Entity::Entity(float x, float y)
    : position(x, y)
    , velocity(0.0f, 0.0f)
    , state(EntityState::Idle)
    , boundingBox(0.0f, 0.0f, 32.0f, 32.0f)
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