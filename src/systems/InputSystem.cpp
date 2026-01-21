#include "InputSystem.h"

InputSystem::InputSystem() {
    actionKeyMap = {
        {sf::Keyboard::Space, EntityState::Attack},
        {sf::Keyboard::E, EntityState::Axe},
        {sf::Keyboard::R, EntityState::Roll},
        {sf::Keyboard::C, EntityState::Hammering},
        {sf::Keyboard::Q, EntityState::Watering},
        {sf::Keyboard::F, EntityState::Dig},
        {sf::Keyboard::G, EntityState::Mining},
    };
}

void InputSystem::update() {
    updateKey(sf::Keyboard::W);
    updateKey(sf::Keyboard::A);
    updateKey(sf::Keyboard::S);
    updateKey(sf::Keyboard::D);

    updateKey(sf::Keyboard::LShift);

    for (const auto& [key, action] : actionKeyMap) {
        updateKey(key);
    }
}

void InputSystem::updateKey(sf::Keyboard::Key key) {
    previousKeyState[key] = currentKeyState[key];
    currentKeyState[key] = sf::Keyboard::isKeyPressed(key);
}

bool InputSystem::wasKeyJustPressed(sf::Keyboard::Key key) const {
    auto prev = previousKeyState.find(key);
    auto curr = currentKeyState.find(key);

    bool wasPreviouslyPressed = (prev != previousKeyState.end()) && prev->second;
    bool isCurrentlyPressed = (curr != currentKeyState.end()) && curr->second;

    return isCurrentlyPressed && !wasPreviouslyPressed;
}

sf::Vector2f InputSystem::getMoveDirection() const {
    sf::Vector2f direction(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
      ) {
        direction.x += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        ) {
        direction.x -= 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)
        ) {
        direction.y += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
        ) {
        direction.y -= 1.0f;
    }

    return direction;
}

bool InputSystem::isRunning() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

bool InputSystem::isActionPressed(EntityState action) const {
    for (const auto& [key, mappedAction] : actionKeyMap) {
        if (mappedAction == action && wasKeyJustPressed(key)) {
            return true;
        }
    }
    return false;
}

EntityState InputSystem::getPressedAction() const {
    for (const auto& [key, action] : actionKeyMap) {
        if (wasKeyJustPressed(key)) {
            return action;
        }
    }
    return EntityState::Idle;
}