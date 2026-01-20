#include "WorldObject.h"
#include "ObjectAtlas.h"
#include "core/Config.h"


sf::Vector2f WorldObject::getWorldPosition(int chunkX, int chunkY) const {

    int worldTileX = chunkX * CHUNK_SIZE + tileX;
    int worldTileY = chunkY * CHUNK_SIZE + tileY;

    return sf::Vector2f(
        (worldTileX + 0.5f) * TILE_SIZE,
        (worldTileY + 1.0f) * TILE_SIZE
    );
}

sf::FloatRect WorldObject::getCollisionBounds(int chunkX, int chunkY) const {
    const ObjectVisual& visual = getObjectVisual(type);

    if (!visual.hasCollision) {
        return sf::FloatRect(0, 0, 0, 0);  // sin colisión
    }

    sf::Vector2f worldPos = getWorldPosition(chunkX, chunkY);

    return sf::FloatRect(
        worldPos.x + visual.hitbox.left,
        worldPos.y + visual.hitbox.top,
        visual.hitbox.width,
        visual.hitbox.height
    );
}

bool WorldObject::hasCollision() const {
    const ObjectVisual& visual = getObjectVisual(type);
    return visual.hasCollision;
}

