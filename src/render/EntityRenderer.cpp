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

    if (debugMode) {
        drawEntityHitbox(window, hero.getBounds());
    }
}

void EntityRenderer::drawEnemy(sf::RenderWindow& window, const Enemy& enemy) {
    const Animation* animation = enemy.getCurrentAnimation();
    if (!animation) {
        return;
    }

    drawAnimation(window, animation, enemy.getPosition(), enemy.isFacingLeft());

    if (debugMode) {
        drawEntityHitbox(window, enemy.getBounds());
    }
}

void EntityRenderer::drawEntityHitbox(sf::RenderWindow& window, const sf::FloatRect& bounds) const {
    sf::RectangleShape hitbox;
    hitbox.setPosition(bounds.left, bounds.top);
    hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setFillColor(sf::Color(0, 255, 0, 80));
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(1.0f);

    window.draw(hitbox);
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


        float spriteOriginX = frameSize.x / 2.0f;
        float spriteOriginY = frameSize.y;

        sprites[i].setOrigin(spriteOriginX, spriteOriginY);
        sprites[i].setPosition(position);

        if (facingLeft) {
            sprites[i].setScale(-1.0f, 1.0f);
        } else {
            sprites[i].setScale(1.0f, 1.0f);
        }

        window.draw(sprites[i]);
    }
}