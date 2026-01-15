#include "Game.h"
#include "core/Config.h"

Game::Game()
    : window(sf::VideoMode(1280, 720), "Save the Valley"),
      world(WORLD_SEED),
      renderer(tileset),
      hero(0.0f, 0.0f)
{
    window.setFramerateLimit(60);

    if (!tileset.load(
        "assets/Sunnyside_World_Assets/Tileset/spr_tileset_sunnysideworld_16px.png"
    )) {
        throw std::runtime_error("Failed to load tileset");
    }

    hero.setPosition(sf::Vector2f(
        CHUNK_SIZE * TILE_SIZE * 2.0f,
        CHUNK_SIZE * TILE_SIZE * 2.0f
    ));

    camera.setSize(sf::Vector2f(1280.0f, 720.0f));
    camera.setPosition(hero.getPosition());
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

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
                running = false;
            }
        }
    }
}

void Game::update(float dt) {
    hero.update(dt);

    world.update(hero.getTileX(), hero.getTileY());

    camera.update(hero.getPosition(), dt);
}

void Game::render() {
    sf::FloatRect cameraBounds = camera.getBounds();
    std::vector<Chunk*> visibleChunks = world.getVisibleChunks(cameraBounds);

    renderer.render(window, camera, visibleChunks, hero);
}