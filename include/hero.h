#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "actions.h"

enum class HeroState { Idle, Walking,Hurt, Death, Attack,Axe,Hamering,Roll,Run,Watering,Dig,Mining,};

class Hero : public sf::Transformable {
private:
    sf::Vector2f velocity;
    float speed = 100.f;
    float actionTimer = 0.f;
    HeroState overrideState = HeroState::Idle;
    bool isOverriding = false;
    std::map<HeroState, std::unique_ptr<Action>> actions;
    HeroState currentState = HeroState::Idle;

public:
    Hero() = default;

    void handleInput(float dt) {
        sf::Vector2f dir(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !isOverriding) {
            overrideState = HeroState::Hamering;
            actionTimer = 0.1f * actions[overrideState]->getFrameCount().x;
            isOverriding = true;
            actions[overrideState]->reset();
        }
        if (dir.x != 0.f || dir.y != 0.f) {
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;
        }
       
        move(dir, dt);
         update(dt, dir);
    }

    void initActions() {
        // WALKING
        std::vector<std::string> walkingPaths = {
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/base_walk_strip8.png",
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/spikeyhair_walk_strip8.png",
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/tools_walk_strip8.png"
        };
        actions[HeroState::Walking] = std::make_unique<Action>(walkingPaths, sf::Vector2i(8, 1));

        // IDLE
        std::vector<std::string> idlePaths = {
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/base_idle_strip9.png",
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/spikeyhair_idle_strip9.png",
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/tools_idle_strip9.png"
        };
        actions[HeroState::Idle] = std::make_unique<Action>(idlePaths, sf::Vector2i(9, 1));

        std::vector<std::string> hamering_paths = {
            "assets/Sunnyside_World_Assets/Characters/Human/HAMMERING/base_hamering_strip23.png",
            "assets/Sunnyside_World_Assets/Characters/Human/HAMMERING/spikeyhair_hamering_strip23.png",
            "assets/Sunnyside_World_Assets/Characters/Human/HAMMERING/tools_hamering_strip23.png"
        };
        actions[HeroState::Hamering] = std::make_unique<Action>(hamering_paths, sf::Vector2i(23, 1));

        actions[HeroState::Walking]->reset();
        actions[HeroState::Walking]->setPosition(getPosition().x, getPosition().y);

        actions[HeroState::Idle]->reset();
        actions[HeroState::Idle]->setPosition(getPosition().x, getPosition().y);

        actions[HeroState::Hamering]->reset();
        actions[HeroState::Hamering]->setPosition(getPosition().x, getPosition().y);

        currentState = HeroState::Idle;
    }

    void update(float dt, const sf::Vector2f& movement) {
       if (isOverriding) {
        actionTimer -= dt;
        if (actionTimer <= 0.f) {
            isOverriding = false;
            currentState = (movement != sf::Vector2f(0.f, 0.f)) 
                ? HeroState::Walking 
                : HeroState::Idle;
        } else {
            currentState = overrideState;
        }
    } else {
        currentState = (movement != sf::Vector2f(0.f, 0.f)) 
            ? HeroState::Walking 
            : HeroState::Idle;
    }

    actions[currentState]->setPosition(getPosition().x, getPosition().y);
    actions[currentState]->update(dt);

    if (velocity.x < 0)
        actions[currentState]->faceLeft();
    else if (velocity.x > 0)
        actions[currentState]->faceRight();
}

    void move(const sf::Vector2f& direction, float dt) {
        velocity = direction * speed;
        sf::Transformable::move(velocity * dt);
    }

    void draw(sf::RenderWindow& window) {
        actions[currentState]->draw(window);
    }
};