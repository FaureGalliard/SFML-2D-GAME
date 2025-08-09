#include <SFML/Graphics.hpp>
#include "hero.h"
#include "map.h" // Tu clase Map

int main() {
    const int GAME_WIDTH = 512;  // Resolución base
    const int GAME_HEIGHT = 288;

    sf::RenderWindow window(sf::VideoMode(GAME_WIDTH * 2, GAME_HEIGHT * 2), "Mi primer juego con SFML");
    window.setFramerateLimit(60);

    // Vista base (para escalado)
    sf::View view(sf::FloatRect(0, 0, GAME_WIDTH, GAME_HEIGHT));
    window.setView(view);

    // Reloj para delta time
    sf::Clock clock;

    // Mapa y héroe
    Map map;

    Hero hero;
    hero.initActions();

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- Update ---
        hero.handleInput(dt);

        // --- Render ---
        window.clear();
        map.draw(window);   // Fondo primero
        hero.draw(window);  // Luego personaje
        window.display();
    }

    return 0;
}
