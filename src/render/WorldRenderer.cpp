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


void WorldRenderer::draw(sf::RenderWindow& window, const Chunk& chunk) {
    sf::Sprite sprite;
    sprite.setTexture(tileset.texture());

    for (int y = 0; y < Chunk::SIZE; ++y) {
        for (int x = 0; x < Chunk::SIZE; ++x) {
            const Tile& tile = chunk.getTile(x, y);

            sprite.setTextureRect(pickTileRect(tile));
            sprite.setPosition(x * 16.f, y * 16.f);

            window.draw(sprite);
        }
    }
}