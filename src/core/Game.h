#pragma once
#include <SFML/Graphics.hpp>

#include "render/WorldRenderer.h"
#include "render/Tileset.h"
#include "world/Chunk.h"
class Game {

public:
    Game();
    void run();
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow window;
    sf::Clock clock;
    bool running = true;

    Tileset tileset;
    WorldRenderer worldRenderer;
    Chunk chunk;
};


