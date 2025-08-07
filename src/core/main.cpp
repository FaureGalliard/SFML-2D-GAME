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

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)  window.close();
        }
        hero.handleInput(dt);
        window.clear();
        hero.draw(window);
        window.display();
    }

        return 0;
}