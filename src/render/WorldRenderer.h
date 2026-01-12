#pragma once
#include <SFML/Graphics.hpp>
#include "world/Chunk.h"
#include "Tileset.h"
#include <vector>
#include "world/world.h"
class WorldRenderer {
public:
    WorldRenderer(const Tileset& tileset);
    void draw(sf::RenderWindow& window,
                         const World& world);
    sf::IntRect pickTileRect(const Tile& tile) const;
private:
    const Tileset& tileset;

    sf::IntRect pickBasic(const Tile& tile) const;
    void drawChunk(sf::RenderWindow& window,const Chunk& chunk);
};


