#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class World;
class Entity;
struct WorldObject;

class CollisionSystem {
public:
    static bool checkWorldObjectCollision(
        const sf::FloatRect& entityBounds,
        const World& world,
        int centerTileX,
        int centerTileY,
        int checkRadius = 2
    );
    
    static std::vector<const WorldObject*> getCollisionObjectsNear(
        const World& world,
        int centerTileX,
        int centerTileY,
        int checkRadius = 2
    );
    
    static sf::Vector2f resolveCollision(
        const sf::FloatRect& entityBounds,
        const sf::FloatRect& objectBounds
    );
    
    static bool intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
        return a.intersects(b);
    }

    static bool canMoveTo(
        const Entity& entity,
        const World& world,
        const sf::Vector2f& newPosition
    );

    static bool checkEntityCollision(
        const Entity& entity1,
        const Entity& entity2
    );
};