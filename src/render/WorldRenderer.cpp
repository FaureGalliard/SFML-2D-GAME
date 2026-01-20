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
    // 🟢 Obtener información visual del tipo
    const ObjectVisual& visual = getObjectVisual(obj.type);

    // 🟢 Clamp variant al rango válido (por seguridad)
    uint8_t variantIndex = obj.variant % visual.variants.size();

    // 🟢 Obtener la variante específica
    const ObjectVariant& sprite = visual.variants[variantIndex];

    // 🟢 Retornar el rect usando la posición y dimensiones
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
}