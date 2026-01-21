#include "Renderer.h"
#include "WorldRenderer.h"
#include "EntityRenderer.h"
#include "Camera.h"
#include "world/Chunk.h"
#include "entities/Hero.h"
#include "entities/Enemy.h"
#include <algorithm>

Renderer::Renderer(const Tileset& tileset)
    : worldRenderer(std::make_unique<WorldRenderer>(tileset)),
      entityRenderer(std::make_unique<EntityRenderer>()) {
}

Renderer::~Renderer() = default;

void Renderer::setDebugMode(bool enabled) {
    debugMode = enabled;
    worldRenderer->setDebugMode(enabled);
    entityRenderer->setDebugMode(enabled);
}

void Renderer::render(sf::RenderWindow& window,
                     const Camera& camera,
                     const std::vector<Chunk*>& visibleChunks,
                     const Hero& hero,
                     const std::vector<const Enemy*>& visibleEnemies) {

    window.clear(sf::Color(50, 120, 180));

    sf::View view = camera.getView();
    window.setView(view);

    worldRenderer->drawTiles(window, visibleChunks);

    std::vector<RenderableObject> renderables;

    for (Chunk* chunk : visibleChunks) {
        for (const WorldObject& obj : chunk->worldObjects) {

            float depthY, leftX;

            if (obj.hasCollision()) {
                sf::FloatRect objBounds = obj.getCollisionBounds(chunk->cx, chunk->cy);
                depthY = objBounds.top + objBounds.height;
                leftX = objBounds.left;
            } else {
                sf::Vector2f worldPos = obj.getWorldPosition(chunk->cx, chunk->cy);
                depthY = worldPos.y;
                leftX = worldPos.x;
            }

            renderables.push_back({
                RenderableType::WorldObject,
                depthY,
                leftX,
                chunk,
                &obj,
                nullptr,
                nullptr
            });
        }
    }

    {
        sf::FloatRect heroBounds = hero.getBounds();
        float heroBottomY = heroBounds.top + heroBounds.height;
        float heroLeftX = heroBounds.left;

        renderables.push_back({
            RenderableType::Hero,
            heroBottomY,
            heroLeftX,
            nullptr,
            nullptr,
            &hero,
            nullptr
        });
    }

    for (const Enemy* enemy : visibleEnemies) {
        sf::FloatRect enemyBounds = enemy->getBounds();
        float enemyBottomY = enemyBounds.top + enemyBounds.height;
        float enemyLeftX = enemyBounds.left;

        renderables.push_back({
            RenderableType::Enemy,
            enemyBottomY,
            enemyLeftX,
            nullptr,
            nullptr,
            nullptr,
            enemy
        });
    }

    std::sort(renderables.begin(), renderables.end(),
        [](const RenderableObject& a, const RenderableObject& b) {

            if (a.depth != b.depth) {
                return a.depth < b.depth;
            }


            return a.x < b.x;
        });

    for (const RenderableObject& renderable : renderables) {
        switch (renderable.type) {
            case RenderableType::WorldObject:
                worldRenderer->drawObject(window, *renderable.chunk, *renderable.worldObject);
                break;

            case RenderableType::Hero:
                entityRenderer->drawHero(window, *renderable.hero);
                break;

            case RenderableType::Enemy:
                entityRenderer->drawEnemy(window, *renderable.enemy);
                break;
        }
    }

    window.display();
}

WorldRenderer& Renderer::getWorldRenderer() {
    return *worldRenderer;
}

EntityRenderer& Renderer::getEntityRenderer() {
    return *entityRenderer;
}