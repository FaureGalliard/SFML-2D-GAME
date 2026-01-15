#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Hero;
class Enemy;


class EntityRenderer {
public:
    EntityRenderer();
    
    void drawHero(sf::RenderWindow& window, const Hero& hero);
    void drawEnemy(sf::RenderWindow& window, const Enemy& enemy);

private:
    sf::CircleShape heroShape;
    sf::CircleShape enemyShape;
};