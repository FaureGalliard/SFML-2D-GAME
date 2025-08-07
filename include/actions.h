#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stdexcept>
class Action {
private:
    std::vector<sf::Sprite> layerSprites;
    std::vector<sf::Texture> layerTextures;
    sf::IntRect currentRect;
    sf::Vector2i frameSize;

    sf::Vector2i frameCount;
    sf::Vector2i currentFrame;
    bool facingLeft = false;
    bool loop = true;
    float frameTime = 0.1f;     
    float timer = 0.f;          

public:
    Action(const std::vector<std::string>& paths, sf::Vector2i _frameCount, float _frameTime = 0.1f, bool _loop = true)
        : frameCount(_frameCount), currentFrame(0, 0), frameTime(_frameTime), loop(_loop) {
        loadSprites(paths);
        updateTextureRect();
    }

    void loadSprites(const std::vector<std::string>& paths) {
        layerTextures.resize(paths.size());
        layerSprites.resize(paths.size());

        for (size_t i = 0; i < paths.size(); ++i) {
            if (!layerTextures[i].loadFromFile(paths[i])) {
                throw std::runtime_error("Failed to load texture: " + paths[i]);
            }
            layerSprites[i].setTexture(layerTextures[i]);
        }
        frameSize = {
            static_cast<int>(layerTextures[0].getSize().x) / frameCount.x,
            static_cast<int>(layerTextures[0].getSize().y) / frameCount.y
        };
    }

    bool update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.f;
            nextFrame();
            return true;
        }
        return false;
    }

    void nextFrame() {
        currentFrame.x++;
        if (currentFrame.x >= frameCount.x) {
            currentFrame.x = 0;
            currentFrame.y++;
            if (currentFrame.y >= frameCount.y) {
                if (loop) {
                    currentFrame.y = 0; 
                } else {
                    currentFrame.y = frameCount.y - 1;
                }
            }
        }
        currentRect = sf::IntRect(
            currentFrame.x * frameSize.x,
            currentFrame.y * frameSize.y,
            frameSize.x,
            frameSize.y
        );

        updateTextureRect();
    }

    void updateTextureRect() {
        
        for (auto& sprite : layerSprites) {
            sprite.setTextureRect(currentRect);
        }
    }

    void setPosition(float x, float y) {
       for (auto& sprite : layerSprites) {
        sprite.setPosition(x, y);
        sprite.setScale(facingLeft ? -1.f : 1.f, 1.f);
        sprite.setOrigin(facingLeft ? frameSize.x : 0.f, 0.f);
    }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& sprite : layerSprites) {
            window.draw(sprite);
        }
    }

    void reset() {
        currentFrame = {0, 0};
        currentRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
        updateTextureRect();
    }

    bool isFinished() const {
        return !loop && currentFrame.x == frameCount.x - 1 && currentFrame.y == frameCount.y - 1;
    }

    void faceLeft() { facingLeft = true; }
    void faceRight() { facingLeft = false; }
    sf::Vector2i getFrameCount() const {
        return frameCount;
    }
    bool isLooping() const { return loop; }
};
