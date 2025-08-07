#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "actions.h"

enum class HeroState { Idle, Walking, Hurt, Death, Attack, Axe, Hammering, Roll, Run, Watering, Dig, Mining };
class Hero : public sf::Transformable {
private:
    sf::Vector2f velocity;
    float speed = 100.f;
    HeroState currentState = HeroState::Idle;
    std::map<HeroState, std::unique_ptr<Action>> actions;

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
        actions[state]->setPosition(getPosition().x, getPosition().y);
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

        // Adjust speed for running
        float currentSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? speed * 2.f : speed;
        velocity = dir * currentSpeed;
        move(velocity * dt);

        // Handle state transitions for actions
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && actions[currentState]->isLooping()) {
            setState(HeroState::Attack);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && actions[currentState]->isLooping()) {
            setState(HeroState::Axe);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && actions[currentState]->isLooping()) {
            setState(HeroState::Roll);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && actions[currentState]->isLooping()) {
            setState(HeroState::Hammering);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && actions[currentState]->isLooping()) {
            setState(HeroState::Watering);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && actions[currentState]->isLooping()) {
            setState(HeroState::Dig);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && actions[currentState]->isLooping()) {
            setState(HeroState::Mining);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H) && actions[currentState]->isLooping()) {
            setState(HeroState::Hurt);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && actions[currentState]->isLooping()) {
            setState(HeroState::Death);
        } else if (!actions[currentState]->isLooping() && actions[currentState]->isFinished()) {
            setState(dir != sf::Vector2f(0.f, 0.f) ? 
                     (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? HeroState::Run : HeroState::Walking) : 
                     HeroState::Idle);
        } else if (actions[currentState]->isLooping()) {
            setState(dir != sf::Vector2f(0.f, 0.f) ? 
                     (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? HeroState::Run : HeroState::Walking) : 
                     HeroState::Idle);
        }

        // Update action and facing direction
        actions[currentState]->update(dt);
        actions[currentState]->setPosition(getPosition().x, getPosition().y);
        if (velocity.x < 0) actions[currentState]->faceLeft();
        else if (velocity.x > 0) actions[currentState]->faceRight();
    }
    
    void setState(HeroState newState) {
        if (currentState != newState) {
            currentState = newState;
            actions[currentState]->reset();
        }
    }

    void draw(sf::RenderWindow& window) {
        actions[currentState]->draw(window);
    }
};