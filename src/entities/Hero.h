#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include <memory>
#include "Entity.h"
#include "core/Config.h"
#include "render/Animation.h"

class Hero : public Entity {
public:
    Hero();
    Hero(float x, float y);

    void update(float dt) override;

    void moveInDirection(const sf::Vector2f& direction, bool running = false);
    void triggerAction(EntityState action);

    int getTileX() const { return static_cast<int>(position.x / TILE_SIZE); }
    int getTileY() const { return static_cast<int>(position.y / TILE_SIZE); }
    sf::Vector2i getTilePosition() const { return {getTileX(), getTileY()}; }

    Animation* getCurrentAnimation();
    const Animation* getCurrentAnimation() const;

    // Verificar si está atacando
    bool isAttacking() const;

private:
    std::map<EntityState, std::unique_ptr<Animation>> animations;
    float speed;
    float runSpeedMultiplier;

    void initAnimations();
    void addAnimation(EntityState state, const std::string& folder,
                     const std::string& file, sf::Vector2i frames,
                     float frameSpeed = 0.1f, bool looping = true);
    void updateAnimations(float dt);
    void handleStateTransitions();
};