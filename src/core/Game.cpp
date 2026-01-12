#include "Game.h"
#include "render/WorldRenderer.h"
#include "core/Config.h"

Game::Game()
    : window(sf::VideoMode(1280, 720), "Save the Valley"),
      tileset(),
      world(),
      worldRenderer(tileset)
{
    window.setFramerateLimit(60);

    if (!tileset.load(
        "assets/Sunnyside_World_Assets/Tileset/spr_tileset_sunnysideworld_16px.png"
    )) {
        throw std::runtime_error("Failed to load tileset");
    }
}

void Game::run() {

    while (window.isOpen() && running) {

        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            running = false;
        }
    }

}

void Game::update(float dt) {

   hero.update(dt);


    world.update(hero.getTileX(), hero.getTileY());

}

void Game::render() {
    window.clear();

    worldRenderer.draw(window, world);
    window.display();
}