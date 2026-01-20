#include "CollisionSystem.h"
#include "world/World.h"
#include "world/Chunk.h"
#include "objects/WorldObject.h"
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