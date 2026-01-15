#include "EntityRenderer.h"
#include "entities/Hero.h"
#include "core/Config.h"

EntityRenderer::EntityRenderer() {
    heroShape.setRadius(TILE_SIZE * 0.4f);
    heroShape.setOrigin(TILE_SIZE * 0.4f, TILE_SIZE * 0.4f);
    heroShape.setFillColor(sf::Color::Green);
    heroShape.setOutlineThickness(2.0f);
    heroShape.setOutlineColor(sf::Color(0, 100, 0));
    
    enemyShape.setRadius(TILE_SIZE * 0.4f);
    enemyShape.setOrigin(TILE_SIZE * 0.4f, TILE_SIZE * 0.4f);
    enemyShape.setFillColor(sf::Color::Red);
    enemyShape.setOutlineThickness(2.0f);
    enemyShape.setOutlineColor(sf::Color(100, 0, 0));
}

void EntityRenderer::drawHero(sf::RenderWindow& window, const Hero& hero) {
    heroShape.setPosition(hero.getPosition());
    window.draw(heroShape);
}

void EntityRenderer::drawEnemy(sf::RenderWindow& window, const Enemy& enemy) {

}