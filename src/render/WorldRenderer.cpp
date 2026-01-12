//
// Created by angel on 8/01/2026.
//

#include "WorldRenderer.h"
#include "Tileset.h"
#include "world/Chunk.h"
#include "world/Tile.h"
#include "world/World.h"

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
            return tileset.get(5, 1);

        default:
            return tileset.get(0, 0);
    }
}

sf::IntRect WorldRenderer::pickObjectRect(const WorldObject& obj) const {
    switch (obj.type) {
        case WorldObjectType::Flower: {
            int cols = 4;          // 31,32,33,34
            int rows = 3;          // y = 1,2,3
            int startX = 31;
            int startY = 1;

            int v = obj.variant % (cols * rows); // aseguras que siempre está en rango

            int tx = startX + (v % cols);        // columna
            int ty = startY + (v / cols);        // fila

            return tileset.get(tx, ty);
        }

        case WorldObjectType::Rock:
            return tileset.get(31 , 4);
        case WorldObjectType::GrassTuft:
            return tileset.get(27 , 1);
        case WorldObjectType::Bush:
            return tileset.get(51 , 4);
        case WorldObjectType::Tree1x3:
            return tileset.get(52 , 3,1,3);
        case WorldObjectType::Tree2x3:
            return tileset.get(51 , 6,2,3);
        default:
            return tileset.get(0, 0);
    }
}


void WorldRenderer::drawWorldObjects(sf::RenderWindow& window,
                                     const Chunk& chunk) {

    sf::Sprite sprite;
    sprite.setTexture(tileset.texture());

    for (const WorldObject& obj : chunk.getWorldObjects()) {

        sf::IntRect tex = pickObjectRect(obj);
        sprite.setTextureRect(tex);

        float wx = (chunk.getCX() * Chunk::SIZE + obj.tileX) * TILE_SIZE;
        float wy = (chunk.getCY() * Chunk::SIZE + obj.tileY) * TILE_SIZE;

        float ox = (TILE_SIZE - tex.width) * 0.5f;
        float oy = (TILE_SIZE - tex.height);

        sprite.setPosition(wx + ox, wy + oy);

        window.draw(sprite);
    }
}

void WorldRenderer::draw(sf::RenderWindow& window, const World& world) {
    sf::RenderStates states;
    states.texture = &tileset.texture();

    for (const auto& [coord, chunkPtr] : world.getChunks()) {
        Chunk& chunk = *chunkPtr;

        if (!chunk.isMeshBuilt())
            chunk.buildMesh(*this);

        window.draw(chunk.getMesh(), states);
        drawWorldObjects(window, chunk);
    }


}
