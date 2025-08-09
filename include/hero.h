#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "actions.h"

enum class HeroState { Idle, Walking, Hurt, Death, Attack, Axe, Hammering, Roll, Run, Watering, Dig, Mining };

class Hero : public sf::Drawable, public sf::Transformable {
private:
    sf::Vector2f velocity;
    float speed = 100.f;
    HeroState currentState = HeroState::Idle;
    std::map<HeroState, std::unique_ptr<Action>> actions;
    bool facingRight = true;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        if (actions.count(currentState)) {
            target.draw(*actions.at(currentState), states);
        }
    }

public:
    Hero() = default;

    void addAction(HeroState state, const std::string& folder, const std::string& file, sf::Vector2i frames, float frameSpeed = 0.1f, bool looping = true) {
        std::string basePath = "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/";
        std::vector<std::string> paths = {
            basePath + "base_" + file,
            basePath + "spikeyhair_" + file,
            basePath + "tools_" + file
        };
        actions[state] = std::make_unique<Action>(paths, frames, frameSpeed, looping);
    }

    void initActions() {
        addAction(HeroState::Idle, "IDLE", "idle_strip9.png", {9, 1});
        addAction(HeroState::Walking, "WALKING", "walk_strip8.png", {8, 1});
        addAction(HeroState::Hammering, "HAMMERING", "hammering_strip23.png", {23, 1}, 0.1f, false);
        addAction(HeroState::Attack, "ATTACK", "attack_strip10.png", {10, 1}, 0.1f, false);
        addAction(HeroState::Axe, "AXE", "axe_strip10.png", {10, 1}, 0.1f, false);
        addAction(HeroState::Roll, "ROLL", "roll_strip10.png", {10, 1}, 0.1f, false);
        addAction(HeroState::Dig, "DIG", "dig_strip13.png", {13, 1}, 0.1f, false);
        addAction(HeroState::Watering, "WATERING", "watering_strip5.png", {5, 1}, 0.1f, false);
        addAction(HeroState::Mining, "MINING", "mining_strip10.png", {10, 1}, 0.1f, false);
        addAction(HeroState::Run, "RUN", "run_strip8.png", {8, 1});
        addAction(HeroState::Hurt, "HURT", "hurt_strip8.png", {8, 1}, 0.1f, false);
        addAction(HeroState::Death, "DEATH", "death_strip13.png", {13, 1}, 0.1f, false);
    }

    void handleInput(float dt) {
        sf::Vector2f dir(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;

        float currentSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? speed * 2.f : speed;
        velocity = dir * currentSpeed;
        move(velocity * dt);

        Action* currentAction = actions[currentState].get();

        const std::vector<std::pair<sf::Keyboard::Key, HeroState>> keyActions = {
            {sf::Keyboard::Space, HeroState::Attack},
            {sf::Keyboard::E,     HeroState::Axe},
            {sf::Keyboard::R,     HeroState::Roll},
            {sf::Keyboard::C,     HeroState::Hammering},
            {sf::Keyboard::Q,     HeroState::Watering},
            {sf::Keyboard::F,     HeroState::Dig},
            {sf::Keyboard::G,     HeroState::Mining},
            {sf::Keyboard::H,     HeroState::Hurt},
            {sf::Keyboard::J,     HeroState::Death}
        };

        bool actionTriggered = false;

        if (currentAction->isLooping()) {
            for (auto& [key, state] : keyActions) {
                if (sf::Keyboard::isKeyPressed(key)) {
                    setState(state);
                    currentAction = actions[currentState].get(); 
                    actionTriggered = true;
                    break;
                }
            }
        }

        if (!actionTriggered) {
            bool moving = dir != sf::Vector2f(0.f, 0.f);
            HeroState targetState =
                moving ? (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? HeroState::Run : HeroState::Walking)
                       : HeroState::Idle;

            if ((!currentAction->isLooping() && currentAction->isFinished()) ||
                currentAction->isLooping()) {
                setState(targetState);
                currentAction = actions[currentState].get();
            }
        }

        currentAction->update(dt);
        currentAction->setPosition(getPosition()); 
        if (velocity.x < 0) facingRight = false, currentAction->faceLeft();
        else if (velocity.x > 0) facingRight = true, currentAction->faceRight();
        else {
            if (facingRight) currentAction->faceRight();
            else currentAction->faceLeft();
        }
    }

    void setState(HeroState newState) {
        if (currentState != newState) {
            currentState = newState;
            auto* action = actions[currentState].get();
            action->reset();
            action->setPosition(getPosition());
        }
    }
};
