#include "Animation.h"
#include <stdexcept>

Animation::Animation(const std::vector<std::string>& texturePaths, 
                     sf::Vector2i frameCount, 
                     float frameTime, 
                     bool looping)
    : frameCount(frameCount)
    , currentFrame(0, 0)
    , frameTime(frameTime)
    , timer(0.0f)
    , loop(looping)
{
    textures.resize(texturePaths.size());

    for (size_t i = 0; i < texturePaths.size(); ++i) {
        if (!textures[i].loadFromFile(texturePaths[i])) {
            throw std::runtime_error("Failed to load texture: " + texturePaths[i]);
        }
    }

    if (!textures.empty()) {
        frameSize = {
            static_cast<int>(textures[0].getSize().x) / frameCount.x,
            static_cast<int>(textures[0].getSize().y) / frameCount.y
        };
    }
}

void Animation::update(float dt) {
    timer += dt;
    
    if (timer < frameTime) {
        return;
    }
    
    timer = 0.0f;
    
    currentFrame.x++;
    
    if (currentFrame.x >= frameCount.x) {
        currentFrame.x = 0;
        currentFrame.y++;
        
        if (currentFrame.y >= frameCount.y) {
            if (loop) {
                currentFrame.y = 0;
            } else {
                // Mantener en el último frame si no es loop
                currentFrame.y = frameCount.y - 1;
                currentFrame.x = frameCount.x - 1;
            }
        }
    }
}

void Animation::reset() {
    currentFrame = {0, 0};
    timer = 0.0f;
}

bool Animation::isFinished() const {
    if (loop) {
        return false;
    }
    return currentFrame.y == frameCount.y - 1 && 
           currentFrame.x == frameCount.x - 1;
}

bool Animation::isLooping() const {
    return loop;
}

sf::IntRect Animation::getCurrentFrame() const {
    return sf::IntRect(
        currentFrame.x * frameSize.x,
        currentFrame.y * frameSize.y,
        frameSize.x,
        frameSize.y
    );
}

sf::Vector2i Animation::getFrameSize() const {
    return frameSize;
}