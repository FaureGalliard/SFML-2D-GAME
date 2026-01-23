#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class Hero;
class Enemy;
class Animation;

class EntityRenderer {
public:
    EntityRenderer();

    void drawHero(sf::RenderWindow& window, const Hero& hero);
    void drawEnemy(sf::RenderWindow& window, const Enemy& enemy);

    void setDebugMode(bool enabled) { debugMode = enabled; }
    bool isDebugMode() const { return debugMode; }

private:
    std::vector<sf::Sprite> sprites;
    bool debugMode = false;

    void drawAnimation(sf::RenderWindow& window,
                      const Animation* animation,
                      const sf::Vector2f& position,
                      bool facingLeft);

    void drawEntityHitbox(sf::RenderWindow& window, const sf::FloatRect& bounds) const;
    void drawAttackHitbox(sf::RenderWindow& window, const sf::FloatRect& bounds) const;
    void drawHealthBar(sf::RenderWindow& window,
                      const sf::Vector2f& position,
                      int currentHealth,
                      int maxHealth) const;
};