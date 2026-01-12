#pragma once
#include <SFML/Graphics.hpp>

#include "worldobjects/WorldObject.h"

class Chunk;
class World;
class Tileset;
struct Tile;

class WorldRenderer {
public:
    WorldRenderer(const Tileset& tileset);
    void draw(sf::RenderWindow& window, const World& world);
    sf::IntRect pickTileRect(const Tile& tile) const;
    void drawWorldObjects(sf::RenderWindow& window, const Chunk& chunk);
private:
    const Tileset& tileset;
    sf::IntRect pickObjectRect(const WorldObject& obj) const;
    sf::IntRect pickBasic(const Tile& tile) const;
};


