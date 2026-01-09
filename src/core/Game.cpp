#include "Game.h"
#include "render/WorldRenderer.h"
#include "world/chunk.h"
Game::Game() : window(sf::VideoMode(1280, 720), L"Save the Valley"),tileset(),worldRenderer(tileset),chunk(0,0)
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

}

void Game::render() {
    window.clear();

    worldRenderer.draw(window, chunk);
    window.display();
}