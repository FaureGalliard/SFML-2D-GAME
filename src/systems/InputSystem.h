#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include "entities/Entity.h"

class InputSystem {
public:
    InputSystem();

    void update();

    sf::Vector2f getMoveDirection() const;
    bool isRunning() const;

    bool isActionPressed(EntityState action) const;
    
    EntityState getPressedAction() const;

private:
    std::map<sf::Keyboard::Key, bool> previousKeyState;
    std::map<sf::Keyboard::Key, bool> currentKeyState;
    
    std::map<sf::Keyboard::Key, EntityState> actionKeyMap;

    void updateKey(sf::Keyboard::Key key);
    bool wasKeyJustPressed(sf::Keyboard::Key key) const;
};