#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stdexcept>
class Action {
private:
    std::vector<sf::Sprite> spritecompleto;
    
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Texture texture2;
    sf::Sprite sprite2;

    sf::Texture texture3;
    sf::Sprite sprite3;

    sf::Vector2i frameCount;
    sf::Vector2i currentFrame;

    float frameTime = 0.1f;     
    float timer = 0.f;          

public:
    Action(const std::string& path, const std::string& path2, const std::string& path3, sf::Vector2i _frameCount)
        : frameCount(_frameCount), currentFrame(0, 0)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture);

        texture2.loadFromFile(path2);
        sprite2.setTexture(texture2);

        texture3.loadFromFile(path3);
        sprite3.setTexture(texture3);

        spritecompleto.push_back(sprite);
        spritecompleto.push_back(sprite2);
        spritecompleto.push_back(sprite3);

        updateTextureRect();
    }

    void update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.f;
            nextFrame();
        }
    }

    void nextFrame() {
        currentFrame.x++;
        if (currentFrame.x >= frameCount.x) {
            currentFrame.x = 0;
            currentFrame.y++;
            if (currentFrame.y >= frameCount.y) {
                currentFrame.y = 0; 
            }
        }
        updateTextureRect();
    }

    void updateTextureRect() {
        int frameWidth = texture.getSize().x / frameCount.x;
        int frameHeight = texture.getSize().y / frameCount.y;

        for (auto& sprite : spritecompleto) {
            sprite.setTextureRect(sf::IntRect(currentFrame.x * frameWidth,
                                                currentFrame.y * frameHeight,
                                                frameWidth, frameHeight));
        }
    }

    void setPosition(float x, float y) {
        for (auto& sprite : spritecompleto) {
            sprite.setPosition(x, y);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& sprite : spritecompleto) {
            window.draw(sprite);
        }
    }

    void reset() {
        currentFrame = {0, 0};
        updateTextureRect();
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    void faceLeft() {
        for (auto& sprite : spritecompleto) {
            sprite.setScale(-1.f, 1.f);
            sprite.setOrigin(sprite.getGlobalBounds().width, 0.f);
        }   
    }

    void faceRight() {
        for (auto& sprite : spritecompleto) {
            sprite.setScale(1.f, 1.f);
            sprite.setOrigin(0.f, 0.f);
        }
    }
};

class Hero : public sf::Transformable {
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed = 100.f;

    std::string currentHair;
    std::string currentAction;

    Action* current;

public:

    Hero() : current(nullptr) {}

     void initActions() {
        current = new Action("assets/Sunnyside_World_Assets/Characters/Human/WALKING/base_walk_strip8.png",
             "assets/Sunnyside_World_Assets/Characters/Human/WALKING/spikeyhair_walk_strip8.png",
             "assets/Sunnyside_World_Assets/Characters/Human/WALKING/tools_walk_strip8.png", sf::Vector2i(8, 1));
        current->setPosition(position.x, position.y);
    }
     void update(float dt) {
        if (current) {
            current->update(dt);
            current->setPosition(position.x, position.y);

            if(velocity.x < 0) {
                current->faceLeft();
            } else if (velocity.x > 0) {
                current->faceRight();
            }
        }
    }

    void move(const sf::Vector2f& direction) {
        velocity = direction * speed;
        position += direction * speed;
    }

    void draw(sf::RenderWindow& window) {
        if (current)
            current->draw(window);
    }

    ~Hero() {
        delete current;
    }
};
