#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

class Hero;
class Enemy;
class Animation;

class EntityRenderer {
public:
    EntityRenderer();

    void drawHero(sf::RenderWindow& window, const Hero& hero);
    void drawEnemy(sf::RenderWindow& window, const Enemy& enemy);

private:
    std::vector<sf::Sprite> sprites;

    void drawAnimation(sf::RenderWindow& window,
                      const Animation* animation,
                      const sf::Vector2f& position,
                      bool facingLeft);
};