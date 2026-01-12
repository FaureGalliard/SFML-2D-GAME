//
// Created by angel on 8/01/2026.
//

#include "WorldRenderer.h"
#include <cstdlib>
#include "world/Tile.h"

WorldRenderer::WorldRenderer(const Tileset& tileset) : tileset(tileset){


}

sf::IntRect WorldRenderer::pickTileRect(const Tile& tile) const {

    if (tile.type == TileType::Water) {
        switch (tile.mask) {
            case 0:  return tileset.get(4, 1);
            case 1:  return tileset.get(4, 1);
            case 2:  return tileset.get(4, 1);
            case 3:  return tileset.get(4, 1);
            case 15: return tileset.get(4, 1);
            default: return tileset.get(4, 1);
        }
    }
    return pickBasic(tile);

}

sf::IntRect WorldRenderer::pickBasic(const Tile& tile) const {
    switch (tile.type) {
        case TileType::Grass:
            return tile.variant == 0
                ? tileset.get(1, 1)
                : tileset.get(2, 1);

        case TileType::Sand:
            return tileset.get(3, 1);

        default:
            return tileset.get(0, 0);
    }
}

void WorldRenderer::drawChunk(sf::RenderWindow& window,
                              const Chunk& chunk) {

    sf::RenderStates states;
    states.texture = &tileset.texture();

    window.draw(chunk.getMesh(), states);
}

void WorldRenderer::draw(sf::RenderWindow& window,
                         const World& world) {

    sf::RenderStates states;
    states.texture = &tileset.texture();

    for (const auto& pair : world.getChunks()) {
        Chunk& chunk = *pair.second;

        if (!chunk.isMeshBuilt()) {
            chunk.buildMesh(*this);
        }

        window.draw(chunk.getMesh(), states);
    }
}