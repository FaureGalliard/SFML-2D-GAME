#include "EntityRenderer.h"
#include "entities/Hero.h"
#include "entities/Enemy.h"
#include "render/Animation.h"
#include "systems/CombatSystem.h"

EntityRenderer::EntityRenderer() {
    sprites.resize(3);
}

void EntityRenderer::drawHero(sf::RenderWindow& window, const Hero& hero) {
    const Animation* animation = hero.getCurrentAnimation();
    if (!animation) {
        return;
    }

    if (hero.isInvulnerable()) {
        static float blinkTimer = 0.0f;
        blinkTimer += 1.0f / 60.0f;
        if (static_cast<int>(blinkTimer * 10) % 2 == 0) {
            if (debugMode) {
                if (hero.isAttacking()) {
                    sf::FloatRect attackHitbox = CombatSystem::getAttackHitbox(hero, hero.getState());
                    drawAttackHitbox(window, attackHitbox);
                }
            }
            return;
        }
    }

    drawAnimation(window, animation, hero.getPosition(), hero.isFacingLeft());

    if (debugMode) {
        drawEntityHitbox(window, hero.getBounds());

        if (hero.isAttacking()) {
            sf::FloatRect attackHitbox = CombatSystem::getAttackHitbox(hero, hero.getState());
            drawAttackHitbox(window, attackHitbox);
        }

        drawHealthBar(window, hero.getPosition(), hero.getHealth(), hero.getMaxHealth());
    }
}

void EntityRenderer::drawEnemy(sf::RenderWindow& window, const Enemy& enemy) {
    const Animation* animation = enemy.getCurrentAnimation();
    if (!animation) {
        return;
    }

    if (enemy.isInvulnerable()) {
        static float blinkTimer = 0.0f;
        blinkTimer += 1.0f / 60.0f;
        if (static_cast<int>(blinkTimer * 10) % 2 == 0) {
            if (debugMode) {
                drawEntityHitbox(window, enemy.getBounds());
            }
            return;
        }
    }

    drawAnimation(window, animation, enemy.getPosition(), enemy.isFacingLeft());

    if (debugMode) {
        drawEntityHitbox(window, enemy.getBounds());
        drawHealthBar(window, enemy.getPosition(), enemy.getHealth(), enemy.getMaxHealth());
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

void EntityRenderer::drawAttackHitbox(sf::RenderWindow& window, const sf::FloatRect& bounds) const {
    sf::RectangleShape hitbox;
    hitbox.setPosition(bounds.left, bounds.top);
    hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setFillColor(sf::Color(255, 0, 0, 100));
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2.0f);

    window.draw(hitbox);
}

void EntityRenderer::drawHealthBar(sf::RenderWindow& window,
                                   const sf::Vector2f& position,
                                   int currentHealth,
                                   int maxHealth) const {
    if (maxHealth <= 0) return;

    float barWidth = 40.0f;
    float barHeight = 5.0f;
    float healthPercent = static_cast<float>(currentHealth) / static_cast<float>(maxHealth);

    sf::RectangleShape background;
    background.setPosition(position.x - barWidth / 2.0f, position.y - 55.0f);
    background.setSize(sf::Vector2f(barWidth, barHeight));
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.0f);

    sf::RectangleShape healthBar;
    healthBar.setPosition(position.x - barWidth / 2.0f, position.y - 55.0f);
    healthBar.setSize(sf::Vector2f(barWidth * healthPercent, barHeight));

    if (healthPercent > 0.6f) {
        healthBar.setFillColor(sf::Color::Green);
    } else if (healthPercent > 0.3f) {
        healthBar.setFillColor(sf::Color::Yellow);
    } else {
        healthBar.setFillColor(sf::Color::Red);
    }

    window.draw(background);
    window.draw(healthBar);
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