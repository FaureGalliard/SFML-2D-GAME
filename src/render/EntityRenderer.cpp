#include "EntityRenderer.h"
#include "entities/Hero.h"
#include "entities/Enemy.h"
#include "render/Animation.h"

EntityRenderer::EntityRenderer() {
    sprites.resize(3);
}

void EntityRenderer::drawHero(sf::RenderWindow& window, const Hero& hero) {
    const Animation* animation = hero.getCurrentAnimation();
    if (!animation) {
        return;
    }

    drawAnimation(window, animation, hero.getPosition(), hero.isFacingLeft());
}

void EntityRenderer::drawEnemy(sf::RenderWindow& window, const Enemy& enemy) {

}

void EntityRenderer::drawAnimation(sf::RenderWindow& window,
                                   const Animation* animation,
                                   const sf::Vector2f& position,
                                   bool facingLeft)
{
    if (!animation) {
        return;
    }

    const auto& textures = animation->getTextures();
    sf::IntRect frameRect = animation->getCurrentFrame();
    sf::Vector2i frameSize = animation->getFrameSize();

    if (sprites.size() < textures.size()) {
        sprites.resize(textures.size());
    }

    for (size_t i = 0; i < textures.size(); ++i) {
        sprites[i].setTexture(textures[i]);
        sprites[i].setTextureRect(frameRect);
        sprites[i].setPosition(position);

        if (facingLeft) {
            sprites[i].setScale(-1.0f, 1.0f);
            sprites[i].setOrigin(static_cast<float>(frameSize.x), 0.0f);
        } else {
            sprites[i].setScale(1.0f, 1.0f);
            sprites[i].setOrigin(0.0f, 0.0f);
        }

        window.draw(sprites[i]);
    }
}