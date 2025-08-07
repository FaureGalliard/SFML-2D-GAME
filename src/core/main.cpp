//main.cpp
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "hero.h"
#include "utils.h"
int main() {
    const int GAME_WIDTH = 320;
    const int GAME_HEIGHT = 180;
    Hero hero;
    hero.initActions();
    sf::Clock clock;


    sf::RenderWindow window(sf::VideoMode(GAME_WIDTH*4, GAME_HEIGHT*4), "Mi primer juego con SFML");
    window.setFramerateLimit(60);
    sf::View view(sf::FloatRect(0, 0, GAME_WIDTH, GAME_HEIGHT));
    window.setView(view);
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        hero.update(dt);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)  window.close();
        }
        sf::Vector2f dir(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;
        hero.move(dir * dt);   
        hero.update(dt);
        window.clear();
        hero.draw(window);
        window.display();
    }

        return 0;
}