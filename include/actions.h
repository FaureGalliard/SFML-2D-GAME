#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stdexcept>

class Action : public sf::Drawable, public sf::Transformable {
private:
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Texture> textures;
    sf::Vector2i frameSize, frameCount, currentFrame{0, 0};
    float frameTime, timer = 0.f;
    bool loop, facingLeft = false;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        for (const auto& s : sprites)
            target.draw(s, states);
    }

    void applyFrame() {
        sf::IntRect rect(
            currentFrame.x * frameSize.x,
            currentFrame.y * frameSize.y,
            frameSize.x, frameSize.y
        );
        for (auto& s : sprites) {
            s.setTextureRect(rect);
            s.setScale(facingLeft ? -1.f : 1.f, 1.f);
            s.setOrigin(facingLeft ? frameSize.x : 0.f, 0.f);
        }
    }

public:
    Action(const std::vector<std::string>& paths, sf::Vector2i frames, float ft = 0.1f, bool lp = true)
        : frameCount(frames), frameTime(ft), loop(lp)
    {
        textures.resize(paths.size());
        sprites.resize(paths.size());

        for (size_t i = 0; i < paths.size(); ++i) {
            if (!textures[i].loadFromFile(paths[i]))
                throw std::runtime_error("Failed to load: " + paths[i]);
            sprites[i].setTexture(textures[i]);
        }

        frameSize = {
            static_cast<int>(textures[0].getSize().x) / frameCount.x,
            static_cast<int>(textures[0].getSize().y) / frameCount.y
        };
        applyFrame();
    }

    bool update(float dt) {
        timer += dt;
        if (timer < frameTime) return false;
        timer = 0.f;

        if (++currentFrame.x >= frameCount.x) {
            currentFrame.x = 0;
            if (!loop && ++currentFrame.y >= frameCount.y) {
                currentFrame.y = frameCount.y - 1;
                return false;
            }
        }
        applyFrame();
        return true;
    }

    void reset() { currentFrame = {0, 0}; applyFrame(); }
    bool isFinished() const { return !loop && currentFrame.y == frameCount.y - 1 && currentFrame.x == frameCount.x - 1; }
    bool isLooping() const { return loop; }
    void faceLeft() { facingLeft = true; applyFrame(); }
    void faceRight() { facingLeft = false; applyFrame(); }
};
