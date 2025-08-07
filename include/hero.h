#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "actions.h"

enum class HeroState { Idle, Walking, Hurt, Death, Attack, Axe, Hamering, Roll, Run, Watering, Dig, Mining };

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

    void addAction(HeroState state, const std::string& folder, const std::string& name, sf::Vector2i frames, float speed = 0.1f, bool looping = true) {
        std::vector<std::string> paths = {
            "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/base_" + name,
            "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/spikeyhair_" + name,
            "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/tools_" + name
        };
        actions[state] = std::make_unique<Action>(paths, frames, speed, looping);
    }

    void initActions() {
        addAction(HeroState::Walking, "WALKING", "walk_strip8.png", sf::Vector2i(8, 1));
        addAction(HeroState::Idle, "IDLE", "idle_strip9.png", sf::Vector2i(9, 1));
        addAction(HeroState::Hamering, "HAMMERING", "hamering_strip23.png", sf::Vector2i(23, 1), 0.1f, false);

        for (auto& [state, action] : actions) {
            action->reset();
            action->setPosition(getPosition().x, getPosition().y);
        }

        currentState = HeroState::Idle;
    }

    sf::Vector2f processInput() {
        sf::Vector2f dir(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && actions[currentState]->isLooping()) {
            setAction(HeroState::Hamering);
        }

        return dir;
    }

    void move(const sf::Vector2f& direction, float dt) {
        velocity = direction * speed;
        sf::Transformable::move(velocity * dt);
    }

    void updateState(float dt, const sf::Vector2f& direction) {
        if (!actions[currentState]->isLooping() && actions[currentState]->isFinished()) {
            setAction(direction != sf::Vector2f(0.f, 0.f) ? HeroState::Walking : HeroState::Idle);
        } else if (actions[currentState]->isLooping()) {
            if (direction != sf::Vector2f(0.f, 0.f) && currentState != HeroState::Hamering) {
                setAction(HeroState::Walking);
            } else if (direction == sf::Vector2f(0.f, 0.f) && currentState != HeroState::Hamering) {
                setAction(HeroState::Idle);
            }
        }

        actions[currentState]->update(dt);
        actions[currentState]->setPosition(getPosition().x, getPosition().y);

        // Face direction
        if (velocity.x < 0) {
            actions[currentState]->faceLeft();
        } else if (velocity.x > 0) {
            actions[currentState]->faceRight();
        }
    }

    void handleInput(float dt) {
        sf::Vector2f direction = processInput();
        move(direction, dt);
        updateState(dt, direction);
    }

    void setAction(HeroState newState) {
        if (currentState != newState) {
            currentState = newState;
            actions[currentState]->reset();
        }
    }

    void draw(sf::RenderWindow& window) {
        actions[currentState]->draw(window);
    }
};