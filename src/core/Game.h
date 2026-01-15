#pragma once
#include <SFML/Graphics.hpp>
#include "world/World.h"
#include "render/Renderer.h"
#include "render/Camera.h"
#include "render/Tileset.h"
#include "entities/Hero.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::Clock clock;
    bool running = true;

    Tileset tileset;

    World world;
    Camera camera;
    Renderer renderer;

    Hero hero;
};