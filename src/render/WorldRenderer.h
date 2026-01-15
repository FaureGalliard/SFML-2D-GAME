#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "objects/WorldObject.h"

class Chunk;
class Tileset;
struct Tile;


class WorldRenderer {
public:
    explicit WorldRenderer(const Tileset& tileset);
    void draw(sf::RenderWindow& window, const std::vector<Chunk*>& visibleChunks);

    sf::IntRect pickTileRect(const Tile& tile) const;
    sf::IntRect pickObjectRect(const WorldObject& obj) const;

    void drawChunkDebugBounds(sf::RenderWindow& window, const Chunk& chunk) const;

private:
    const Tileset& tileset;
    sf::IntRect pickBasic(const Tile& tile) const;
};