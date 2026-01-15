#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


class Animation {
public:
    Animation(const std::vector<std::string>& texturePaths, 
              sf::Vector2i frameCount, 
              float frameTime = 0.1f, 
              bool looping = true);

    void update(float dt);
    void reset();
    
    bool isFinished() const;
    bool isLooping() const;
    sf::IntRect getCurrentFrame() const;
    sf::Vector2i getFrameSize() const;
    const std::vector<sf::Texture>& getTextures() const { return textures; }

private:
    std::vector<sf::Texture> textures;
    sf::Vector2i frameSize;
    sf::Vector2i frameCount;
    sf::Vector2i currentFrame;
    float frameTime;
    float timer;
    bool loop;

    void calculateCurrentFrame();
};