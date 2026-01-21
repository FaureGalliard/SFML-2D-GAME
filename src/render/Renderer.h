#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>

class Chunk;
class WorldRenderer;
class EntityRenderer;
class Camera;
class Hero;
class Enemy;
class Tileset;
struct WorldObject;

enum class RenderableType {
    WorldObject,
    Hero,
    Enemy
};

struct RenderableObject {
    RenderableType type;
    float depth;
    float x;

    const Chunk* chunk;
    const WorldObject* worldObject;
    const Hero* hero;
    const Enemy* enemy;
};

class Renderer {
public:
    explicit Renderer(const Tileset& tileset);
    ~Renderer();

    void render(sf::RenderWindow& window,
                const Camera& camera,
                const std::vector<Chunk*>& visibleChunks,
                const Hero& hero,
                const std::vector<const Enemy*>& visibleEnemies);

    WorldRenderer& getWorldRenderer();
    EntityRenderer& getEntityRenderer();

    void setDebugMode(bool enabled);
    bool isDebugMode() const { return debugMode; }

private:
    std::unique_ptr<WorldRenderer> worldRenderer;
    std::unique_ptr<EntityRenderer> entityRenderer;
    bool debugMode = false;
};