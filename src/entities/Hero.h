#pragma once
#include <SFML/System/Vector2.hpp>

class Hero {
public:
    Hero();
    void update(float dt);
    int getTileX() const;
    int getTileY() const;

    float getX() const {return position.x;}
    float getY() const {return position.y;}
private:
    sf::Vector2f position;
    float speed = 150.f;
};

