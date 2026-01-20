#include "Hero.h"
#include <cmath>

Hero::Hero()
    : Entity()
    , speed(100.0f)
    , runSpeedMultiplier(2.0f)
{
    initAnimations();
    setState(EntityState::Idle);
}

Hero::Hero(float x, float y)
    : Entity(x, y)
    , speed(100.0f)
    , runSpeedMultiplier(2.0f)
{
    initAnimations();
    setState(EntityState::Idle);
}

void Hero::initAnimations() {
    addAnimation(EntityState::Idle,      "IDLE",      "idle_strip9.png",        {9, 1});
    addAnimation(EntityState::Walking,   "WALKING",   "walk_strip8.png",        {8, 1});
    addAnimation(EntityState::Running,   "RUN",       "run_strip8.png",         {8, 1});
    addAnimation(EntityState::Attack,    "ATTACK",    "attack_strip10.png",     {10, 1}, 0.1f, false);
    addAnimation(EntityState::Axe,       "AXE",       "axe_strip10.png",        {10, 1}, 0.1f, false);
    addAnimation(EntityState::Hammering, "HAMMERING", "hammering_strip23.png",  {23, 1}, 0.1f, false);
    addAnimation(EntityState::Roll,      "ROLL",      "roll_strip10.png",       {10, 1}, 0.1f, false);
    addAnimation(EntityState::Dig,       "DIG",       "dig_strip13.png",        {13, 1}, 0.1f, false);
    addAnimation(EntityState::Watering,  "WATERING",  "watering_strip5.png",    {5, 1}, 0.1f, false);
    addAnimation(EntityState::Mining,    "MINING",    "mining_strip10.png",     {10, 1}, 0.1f, false);
    addAnimation(EntityState::Hurt,      "HURT",      "hurt_strip8.png",        {8, 1}, 0.1f, false);
    addAnimation(EntityState::Death,     "DEATH",     "death_strip13.png",      {13, 1}, 0.1f, false);
}

void Hero::addAnimation(EntityState state,
                       const std::string& folder,
                       const std::string& file,
                       sf::Vector2i frames,
                       float frameSpeed,
                       bool looping)
{
    std::string basePath = "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/";
    std::vector<std::string> paths = {
        basePath + "base_" + file,
        basePath + "spikeyhair_" + file,
        basePath + "tools_" + file
    };
    animations[state] = std::make_unique<Animation>(paths, frames, frameSpeed, looping);
}

void Hero::update(float dt) {
    handleStateTransitions();
    updateAnimations(dt);


    applyCollisions(*world, dt);

    // Actualizar dirección
    if (velocity.x < 0) {
        facingLeft = true;
    } else if (velocity.x > 0) {
        facingLeft = false;
    }
}

void Hero::moveInDirection(const sf::Vector2f& direction, bool running) {
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0) {
        sf::Vector2f normalized = direction / length;
        float currentSpeed = running ? speed * runSpeedMultiplier : speed;
        velocity = normalized * currentSpeed;
    } else {
        velocity = {0.0f, 0.0f};
    }
}

void Hero::triggerAction(EntityState action) {
    if (auto it = animations.find(state); it != animations.end()) {
        if (it->second->isLooping() || it->second->isFinished()) {
            setState(action);
            if (auto newAnim = animations.find(action); newAnim != animations.end()) {
                newAnim->second->reset();
            }
        }
    }
}

void Hero::updateAnimations(float dt) {
    if (auto it = animations.find(state); it != animations.end()) {
        it->second->update(dt);
    }
}

void Hero::handleStateTransitions() {
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

Animation* Hero::getCurrentAnimation() {
    auto it = animations.find(state);
    return it != animations.end() ? it->second.get() : nullptr;
}

const Animation* Hero::getCurrentAnimation() const {
    auto it = animations.find(state);
    return it != animations.end() ? it->second.get() : nullptr;
}