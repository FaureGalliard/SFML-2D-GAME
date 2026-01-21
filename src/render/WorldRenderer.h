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

    void drawTiles(sf::RenderWindow& window, const std::vector<Chunk*>& visibleChunks);
    void drawObject(sf::RenderWindow& window, const Chunk& chunk, const WorldObject& obj);

    void draw(sf::RenderWindow& window, const std::vector<Chunk*>& visibleChunks);

    sf::IntRect pickTileRect(const Tile& tile) const;
    sf::IntRect pickObjectRect(const WorldObject& obj) const;

    void drawChunkDebugBounds(sf::RenderWindow& window, const Chunk& chunk) const;

    void setDebugMode(bool enabled) { debugMode = enabled; }
    bool isDebugMode() const { return debugMode; }

private:
    const Tileset& tileset;
    bool debugMode = false;

    sf::IntRect pickBasic(const Tile& tile) const;
    void drawObjectHitboxes(sf::RenderWindow& window, const std::vector<Chunk*>& visibleChunks) const;
};