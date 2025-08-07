#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stdexcept>

class Action {
private:
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Texture> textures;
    sf::Vector2i frameSize;
    sf::Vector2i frameCount;
    sf::Vector2i currentFrame;
    float frameTime;
    float timer = 0.f;
    bool loop;
    bool facingLeft = false;

public:
    Action(const std::vector<std::string>& paths, sf::Vector2i _frameCount, float _frameTime = 0.1f, bool _loop = true)
        : frameCount(_frameCount), frameTime(_frameTime), loop(_loop) {
        textures.resize(paths.size());
        sprites.resize(paths.size());
        for (size_t i = 0; i < paths.size(); ++i) {
            if (!textures[i].loadFromFile(paths[i])) {
                throw std::runtime_error("Failed to load texture: " + paths[i]);
            }
            sprites[i].setTexture(textures[i]);
        }
        frameSize = {
            static_cast<int>(textures[0].getSize().x) / frameCount.x,
            static_cast<int>(textures[0].getSize().y) / frameCount.y
        };
        updateTextureRect();
    }

    bool update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.f;
            currentFrame.x++;
            if (currentFrame.x >= frameCount.x) {
                currentFrame.x = 0;
                currentFrame.y = loop ? 0 : std::min(currentFrame.y + 1, frameCount.y - 1);
            }
            updateTextureRect();
            return true;
        }
        return false;
    }

    void updateTextureRect() {
        sf::IntRect rect(currentFrame.x * frameSize.x, currentFrame.y * frameSize.y, frameSize.x, frameSize.y);
        for (auto& sprite : sprites) {
            sprite.setTextureRect(rect);
        }
    }

    void setPosition(float x, float y) {
        for (auto& sprite : sprites) {
            sprite.setPosition(x, y);
            sprite.setScale(facingLeft ? -1.f : 1.f, 1.f);
            sprite.setOrigin(facingLeft ? frameSize.x : 0.f, 0.f);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& sprite : sprites) {
            window.draw(sprite);
        }
    }

    void reset() {
        currentFrame = {0, 0};
        updateTextureRect();
    }

    bool isFinished() const {
        return !loop && currentFrame.x == frameCount.x - 1 && currentFrame.y == frameCount.y - 1;
    }

    void faceLeft() { facingLeft = true; updateTextureRect(); }
    void faceRight() { facingLeft = false; updateTextureRect(); }

    bool isLooping() const { return loop; }
};