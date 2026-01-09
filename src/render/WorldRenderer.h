#pragma once
#include <SFML/Graphics.hpp>
#include "world/Chunk.h"
#include "world/Tile.h"
#include "Tileset.h"
#include <vector>

class WorldRenderer {
public:
    WorldRenderer(const Tileset& tileset);
    void draw(sf::RenderWindow& window,const Chunk& chunk);
private:
    const Tileset& tileset;
    sf::IntRect pickTileRect(const Tile& tile) const;
    sf::IntRect pickBasic(const Tile& tile) const;
};


