#include "WorldRenderer.h"
#include "Tileset.h"
#include "world/Chunk.h"
#include "world/Tile.h"
#include "objects/ObjectAtlas.h"

WorldRenderer::WorldRenderer(const Tileset& tileset)
    : tileset(tileset) {
}

sf::IntRect WorldRenderer::pickTileRect(const Tile& tile) const {
    if (tile.type == TileType::Water) {
        return tileset.get(4, 1);
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
    const ObjectVisual& visual = getObjectVisual(obj.type);
    uint8_t variantIndex = obj.variant % visual.variants.size();
    const ObjectVariant& sprite = visual.variants[variantIndex];
    return tileset.get(sprite.x, sprite.y, visual.width, visual.height);
}

void WorldRenderer::drawChunkDebugBounds(sf::RenderWindow& window,
                                          const Chunk& chunk) const {
    sf::RectangleShape rect;
    rect.setSize({
        float(CHUNK_SIZE * TILE_SIZE),
        float(CHUNK_SIZE * TILE_SIZE)
    });
    rect.setPosition(
        float(chunk.cx * CHUNK_SIZE * TILE_SIZE),
        float(chunk.cy * CHUNK_SIZE * TILE_SIZE)
    );
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(2.f);
    rect.setOutlineColor(sf::Color::Red);
    window.draw(rect);
}

void WorldRenderer::drawObjectHitboxes(sf::RenderWindow& window,
                                       const std::vector<Chunk*>& visibleChunks) const {
    for (const Chunk* chunk : visibleChunks) {
        for (const WorldObject& obj : chunk->worldObjects) {
            sf::FloatRect bounds = obj.getCollisionBounds(chunk->cx, chunk->cy);

            if (obj.hasCollision() && bounds.width > 0 && bounds.height > 0) {
                sf::RectangleShape hitbox;
                hitbox.setPosition(bounds.left, bounds.top);
                hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
                hitbox.setFillColor(sf::Color(255, 0, 0, 80));  // Rojo semi-transparente
                hitbox.setOutlineColor(sf::Color::Red);
                hitbox.setOutlineThickness(1.0f);

                window.draw(hitbox);
            }

            sf::Vector2f worldPos = obj.getWorldPosition(chunk->cx, chunk->cy);
            sf::CircleShape center(2.0f);
            center.setPosition(worldPos.x - 2.0f, worldPos.y - 2.0f);
            center.setFillColor(sf::Color::Yellow);
            window.draw(center);
        }
    }
}

void WorldRenderer::draw(sf::RenderWindow& window,
                         const std::vector<Chunk*>& visibleChunks) {
    sf::RenderStates states;
    states.texture = &tileset.texture();

    for (Chunk* chunkPtr : visibleChunks) {
        if (!chunkPtr->isMeshBuilt()) {
            chunkPtr->buildMesh(*this);
        }
        if (!chunkPtr->isObjectMeshBuilt()) {
            chunkPtr->buildObjectMesh(*this);
        }

        window.draw(chunkPtr->mesh, states);
        window.draw(chunkPtr->objectMesh, states);
    }

    if (debugMode) {
        drawObjectHitboxes(window, visibleChunks);
    }
}