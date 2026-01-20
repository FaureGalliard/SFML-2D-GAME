#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class World;

enum class EntityState {
    Idle, Walking, Running, Attack, Hurt, Death,
    Axe, Hammering, Roll, Watering, Dig, Mining
};

class Entity {
public:
    Entity();
    Entity(float x, float y);
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setPosition(float x, float y) { position = {x, y}; }

    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    void setVelocity(float x, float y) { velocity = {x, y}; }

    EntityState getState() const { return state; }
    void setState(EntityState newState);

    sf::FloatRect getBounds() const;
    void setBoundingBox(const sf::FloatRect& box) { boundingBox = box; }

    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    bool isAlive() const { return health > 0; }

    bool isFacingLeft() const { return facingLeft; }
    void setFacingLeft(bool left) { facingLeft = left; }

    void applyCollisions(const World& world, float dt);

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    EntityState state;
    sf::FloatRect boundingBox;
    int health;
    int maxHealth;
    bool facingLeft;

    sf::Vector2f tryMove(const World& world, const sf::Vector2f& newPosition);
};