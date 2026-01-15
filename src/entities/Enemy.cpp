#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
    : Entity()
    , speed(100.0f)
    , runSpeedMultiplier(2.0f)
{
    initAnimations();
    setState(EntityState::Idle);
}

Enemy::Enemy(float x, float y)
    : Entity(x, y)
    , speed(100.0f)
    , runSpeedMultiplier(2.0f)
{
    initAnimations();
    setState(EntityState::Idle);
}

void Enemy::initAnimations() {
    addAnimation(EntityState::Idle,"idle_strip9.png",        {9, 1});
    addAnimation(EntityState::Walking,"walk_strip8.png",        {8, 1});
    addAnimation(EntityState::Attack,"attack_strip10.png",     {10, 1}, 0.1f, false);
    addAnimation(EntityState::Hurt,"hurt_strip8.png",        {8, 1}, 0.1f, false);
    addAnimation(EntityState::Death,"death_strip13.png",      {13, 1}, 0.1f, false);
}

void Enemy::addAnimation(EntityState state,
                       const std::string& file,
                       sf::Vector2i frames,
                       float frameSpeed,
                       bool looping) {
    std::vector<std::string> paths = {"assets/Sunnyside_World_Assets/Characters/Skeleton/PNG/" + file};

    animations[state] = std::make_unique<Animation>(paths, frames, frameSpeed, looping);
}

void Enemy::update(float dt) {
    handleStateTransitions();

    updateAnimations(dt);

    position += velocity * dt;

    if (velocity.x < 0) {
        facingLeft = true;
    } else if (velocity.x > 0) {
        facingLeft = false;
    }
}

void Enemy::moveInDirection(const sf::Vector2f& direction, bool running) {
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0) {
        sf::Vector2f normalized = direction / length;
        float currentSpeed = running ? speed * runSpeedMultiplier : speed;
        velocity = normalized * currentSpeed;
    } else {
        velocity = {0.0f, 0.0f};
    }
}

void Enemy::triggerAction(EntityState action) {
    if (auto it = animations.find(state); it != animations.end()) {
        if (it->second->isLooping() || it->second->isFinished()) {
            setState(action);
            if (auto newAnim = animations.find(action); newAnim != animations.end()) {
                newAnim->second->reset();
            }
        }
    }
}

void Enemy::updateAnimations(float dt) {
    if (auto it = animations.find(state); it != animations.end()) {
        it->second->update(dt);
    }
}

void Enemy::handleStateTransitions() {
    if (auto it = animations.find(state); it != animations.end()) {
        if (!it->second->isLooping() && it->second->isFinished()) {
            float velLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            EntityState newState;
            if (velLength > speed * 1.5f) {
                newState = EntityState::Running;
            } else if (velLength > 0.1f) {
                newState = EntityState::Walking;
            } else {
                newState = EntityState::Idle;
            }

            if (newState != state) {
                setState(newState);
                if (auto newAnim = animations.find(state); newAnim != animations.end()) {
                    newAnim->second->reset();
                }
            }
        }
        else if (it->second->isLooping()) {
            float velLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            EntityState desiredState;
            if (velLength > speed * 1.5f) {
                desiredState = EntityState::Running;
            } else if (velLength > 0.1f) {
                desiredState = EntityState::Walking;
            } else {
                desiredState = EntityState::Idle;
            }

            if (desiredState != state) {
                setState(desiredState);
                if (auto newAnim = animations.find(state); newAnim != animations.end()) {
                    newAnim->second->reset();
                }
            }
        }
    }
}

Animation* Enemy::getCurrentAnimation() {
    auto it = animations.find(state);
    return it != animations.end() ? it->second.get() : nullptr;
}

const Animation* Enemy::getCurrentAnimation() const {
    auto it = animations.find(state);
    return it != animations.end() ? it->second.get() : nullptr;
}