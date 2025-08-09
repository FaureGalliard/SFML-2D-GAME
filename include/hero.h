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

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        if (auto it = actions.find(currentState); it != actions.end())
            target.draw(*it->second, states);
    }

    void addAction(HeroState state, const std::string& folder, const std::string& file,
                   sf::Vector2i frames, float frameSpeed = 0.1f, bool looping = true)
    {
        std::string basePath = "assets/Sunnyside_World_Assets/Characters/Human/" + folder + "/";
        std::vector<std::string> paths = {
            basePath + "base_" + file,
            basePath + "spikeyhair_" + file,
            basePath + "tools_" + file
        };
        actions[state] = std::make_unique<Action>(paths, frames, frameSpeed, looping);
    }

public:
    Hero(){
        initActions();
    }

    void initActions() {
        addAction(HeroState::Idle,      "IDLE",      "idle_strip9.png",        {9, 1});
        addAction(HeroState::Walking,   "WALKING",   "walk_strip8.png",        {8, 1});
        addAction(HeroState::Hammering, "HAMMERING", "hammering_strip23.png",  {23, 1}, 0.1f, false);
        addAction(HeroState::Attack,    "ATTACK",    "attack_strip10.png",     {10, 1}, 0.1f, false);
        addAction(HeroState::Axe,       "AXE",       "axe_strip10.png",        {10, 1}, 0.1f, false);
        addAction(HeroState::Roll,      "ROLL",      "roll_strip10.png",       {10, 1}, 0.1f, false);
        addAction(HeroState::Dig,       "DIG",       "dig_strip13.png",        {13, 1}, 0.1f, false);
        addAction(HeroState::Watering,  "WATERING",  "watering_strip5.png",    {5, 1}, 0.1f, false);
        addAction(HeroState::Mining,    "MINING",    "mining_strip10.png",     {10, 1}, 0.1f, false);
        addAction(HeroState::Run,       "RUN",       "run_strip8.png",         {8, 1});
        addAction(HeroState::Hurt,      "HURT",      "hurt_strip8.png",        {8, 1}, 0.1f, false);
        addAction(HeroState::Death,     "DEATH",     "death_strip13.png",      {13, 1}, 0.1f, false);
    }

    void handleInput(float dt) {
        sf::Vector2f dir(
            (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1.f : 0.f) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1.f : 0.f),
            (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1.f : 0.f) - (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1.f : 0.f)
        );

        velocity = dir * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? speed * 2.f : speed);
        move(velocity * dt);

        Action* action = actions[currentState].get();

        const std::map<sf::Keyboard::Key, HeroState> keyMap = {
            {sf::Keyboard::Space, HeroState::Attack}, {sf::Keyboard::E, HeroState::Axe},
            {sf::Keyboard::R, HeroState::Roll},       {sf::Keyboard::C, HeroState::Hammering},
            {sf::Keyboard::Q, HeroState::Watering},   {sf::Keyboard::F, HeroState::Dig},
            {sf::Keyboard::G, HeroState::Mining},     {sf::Keyboard::H, HeroState::Hurt},
            {sf::Keyboard::J, HeroState::Death}
        };

        bool triggered = false;
        if (action->isLooping()) {
            for (auto& [key, state] : keyMap) {
                if (sf::Keyboard::isKeyPressed(key)) {
                    setState(state);
                    action = actions[currentState].get();
                    triggered = true;
                    break;
                }
            }
        }

        if (!triggered) {
            HeroState target = (dir != sf::Vector2f(0.f, 0.f))
                ? (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? HeroState::Run : HeroState::Walking)
                : HeroState::Idle;

            if (action->isLooping() || (!action->isLooping() && action->isFinished())) {
                setState(target);
                action = actions[currentState].get();
            }
        }

        action->update(dt);
        action->setPosition(getPosition());

        if (velocity.x < 0) action->faceLeft();
        else if (velocity.x > 0) action->faceRight();
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
