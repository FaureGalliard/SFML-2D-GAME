#pragma once
#include <SFML/Graphics.hpp>

class Chunk;
class World;
class Tileset;
struct Tile;

class WorldRenderer {
public:
    WorldRenderer(const Tileset& tileset);
    void draw(sf::RenderWindow& window, const World& world);
    sf::IntRect pickTileRect(const Tile& tile) const;
private:
    const Tileset& tileset;

    sf::IntRect pickBasic(const Tile& tile) const;
};


