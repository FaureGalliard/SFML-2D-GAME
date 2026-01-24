#include "CollisionSystem.h"
#include "world/World.h"
#include "world/Chunk.h"
#include "objects/WorldObject.h"
#include "entities/Entity.h"
#include "core/Config.h"
#include <cmath>

bool CollisionSystem::checkWorldObjectCollision(
    const sf::FloatRect& entityBounds,
    const World& world,
    int centerTileX,
    int centerTileY,
    int checkRadius)
{
    // Calcular el chunk central
    int centerCx = centerTileX >= 0 ? centerTileX / CHUNK_SIZE : (centerTileX - CHUNK_SIZE + 1) / CHUNK_SIZE;
    int centerCy = centerTileY >= 0 ? centerTileY / CHUNK_SIZE : (centerTileY - CHUNK_SIZE + 1) / CHUNK_SIZE;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int chunkX = centerCx + dx;
            int chunkY = centerCy + dy;

            const Chunk* chunk = world.getChunk(chunkX, chunkY);
            if (!chunk) continue;

            for (const WorldObject& obj : chunk->worldObjects) {
                if (!obj.hasCollision()) continue;

                int worldX = chunkX * CHUNK_SIZE + obj.tileX;
                int worldY = chunkY * CHUNK_SIZE + obj.tileY;

                int distX = std::abs(worldX - centerTileX);
                int distY = std::abs(worldY - centerTileY);

                if (distX <= checkRadius && distY <= checkRadius) {
                    sf::FloatRect objBounds = obj.getCollisionBounds(chunkX, chunkY);
                    if (entityBounds.intersects(objBounds)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

std::vector<const WorldObject*> CollisionSystem::getCollisionObjectsNear(
    const World& world,
    int centerTileX,
    int centerTileY,
    int checkRadius)
{
    std::vector<const WorldObject*> result;

    int centerCx = centerTileX >= 0 ? centerTileX / CHUNK_SIZE : (centerTileX - CHUNK_SIZE + 1) / CHUNK_SIZE;
    int centerCy = centerTileY >= 0 ? centerTileY / CHUNK_SIZE : (centerTileY - CHUNK_SIZE + 1) / CHUNK_SIZE;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int chunkX = centerCx + dx;
            int chunkY = centerCy + dy;

            const Chunk* chunk = world.getChunk(chunkX, chunkY);
            if (!chunk) continue;

            for (const WorldObject& obj : chunk->worldObjects) {
                if (!obj.hasCollision()) continue;

                int worldX = chunkX * CHUNK_SIZE + obj.tileX;
                int worldY = chunkY * CHUNK_SIZE + obj.tileY;

                int distX = std::abs(worldX - centerTileX);
                int distY = std::abs(worldY - centerTileY);

                if (distX <= checkRadius && distY <= checkRadius) {
                    result.push_back(&obj);
                }
            }
        }
    }

    return result;
}

sf::Vector2f CollisionSystem::resolveCollision(
    const sf::FloatRect& entityBounds,
    const sf::FloatRect& objectBounds)
{
    float overlapLeft = (entityBounds.left + entityBounds.width) - objectBounds.left;
    float overlapRight = (objectBounds.left + objectBounds.width) - entityBounds.left;
    float overlapTop = (entityBounds.top + entityBounds.height) - objectBounds.top;
    float overlapBottom = (objectBounds.top + objectBounds.height) - entityBounds.top;

    float minOverlapX = std::min(overlapLeft, overlapRight);
    float minOverlapY = std::min(overlapTop, overlapBottom);

    sf::Vector2f resolution(0.0f, 0.0f);

    if (minOverlapX < minOverlapY) {
        if (overlapLeft < overlapRight) {
            resolution.x = -overlapLeft;
        } else {
            resolution.x = overlapRight;
        }
    } else {
        if (overlapTop < overlapBottom) {
            resolution.y = -overlapTop;
        } else {
            resolution.y = overlapBottom;
        }
    }

    return resolution;
}

bool CollisionSystem::canMoveTo(
    const Entity& entity,
    const World& world,
    const sf::Vector2f& newPosition)
{
    sf::FloatRect localBounds = entity.getLocalBoundingBox();

    sf::FloatRect testBounds(
        newPosition.x + localBounds.left,
        newPosition.y + localBounds.top,
        localBounds.width,
        localBounds.height
    );

    int tileX = static_cast<int>(newPosition.x / TILE_SIZE);
    int tileY = static_cast<int>(newPosition.y / TILE_SIZE);

    return !checkWorldObjectCollision(testBounds, world, tileX, tileY, 2);
}

bool CollisionSystem::checkEntityCollision(
    const Entity& entity1,
    const Entity& entity2)
{
    return entity1.getBounds().intersects(entity2.getBounds());
}