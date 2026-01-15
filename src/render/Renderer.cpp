#include "Renderer.h"
#include "WorldRenderer.h"
#include "EntityRenderer.h"
#include "Camera.h"
#include "world/Chunk.h"

Renderer::Renderer(const Tileset& tileset)
    : worldRenderer(std::make_unique<WorldRenderer>(tileset)),
      entityRenderer(std::make_unique<EntityRenderer>()) {
}

Renderer::~Renderer() = default;

void Renderer::render(sf::RenderWindow& window,
                     const Camera& camera,
                     const std::vector<Chunk*>& visibleChunks,
                     const Hero& hero) {

    window.clear(sf::Color(50, 120, 180)); // Color de fondo azul cielo

    sf::View view = camera.getView();
    window.setView(view);

    worldRenderer->draw(window, visibleChunks);

    entityRenderer->drawHero(window, hero);


    window.display();
}

WorldRenderer& Renderer::getWorldRenderer() {
    return *worldRenderer;
}

EntityRenderer& Renderer::getEntityRenderer() {
    return *entityRenderer;
}