#include "entities/Entity.h"

Entity::Entity()
    : position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , state(EntityState::Idle)
    , boundingBox(-6.0f, -40.0f, 12.0f, 12.0f)
    , health(100)
    , maxHealth(100)
    , facingLeft(false)
    , invulnerable(false)
    , invulnerabilityTimer(0.0f)
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
    , invulnerable(false)
    , invulnerabilityTimer(0.0f)
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

void Entity::takeDamage(int damage) {
    if (invulnerable || !isAlive()) {
        return;
    }

    health -= damage;
    if (health < 0) {
        health = 0;
    }

    // Activar invulnerabilidad temporal
    invulnerable = true;
    invulnerabilityTimer = 0.5f;  // 0.5 segundos de invulnerabilidad
}

void Entity::updateInvulnerability(float dt) {
    if (invulnerable) {
        invulnerabilityTimer -= dt;
        if (invulnerabilityTimer <= 0.0f) {
            invulnerable = false;
            invulnerabilityTimer = 0.0f;
        }
    }
}