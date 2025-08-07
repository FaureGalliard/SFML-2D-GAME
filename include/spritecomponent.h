#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class SpriteComponent: public sf::Drawable{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f currentFrame;
    sf::Vector2f frameSize;
    sf::Vector2f frameCount;
    float frameDuration; 
    float elapsedTime;

public:

    SpriteComponent(const std::string& textureFile,
                    const sf::Vector2f& frameCount
                    ){
        texture.loadFromFile(textureFile);
        sprite.setTexture(texture);
        currentFrame = sf::Vector2f(0, 0);
        frameSize = sf::Vector2f(texture.getSize().x / frameCount.x,
                                 texture.getSize().y / frameCount.y);
        this->frameCount = frameCount;
        frameDuration = 1; 
        
    }

    void update(float dt) {
        elapsedTime += dt;
        if (elapsedTime >= frameDuration) {
            elapsedTime = 0.f;
            advanceFrame();
        }
    }

    void advanceFrame() {
        currentFrame.x++;
        if (currentFrame.x >= frameCount.x) {
            currentFrame.x = 0;
            currentFrame.y++;
            if (currentFrame.y >= frameCount.y) {
                currentFrame.y = 0;
            }
        }
        sprite.setTextureRect(sf::IntRect(
            currentFrame.x * frameSize.x,
            currentFrame.y * frameSize.y,
            frameSize.x, frameSize.y));
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(sprite, states);
    }   
    //setters  

    const sf::Sprite& getSprite() const {
        return sprite;
    }

    sf::Vector2f getCurrentFrame() const {
        return currentFrame;
    }

    sf::Vector2f getFrameSize() const {
        return frameSize;
    }

    sf::Vector2f getFrameCount() const {
        return frameCount;
    }


};
