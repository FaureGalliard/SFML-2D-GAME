#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stdexcept>
class Action {
private:
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Texture> textures;
    int frameWidth = 0;
    int frameHeight = 0;
    sf::Vector2i frameCount;
    sf::Vector2i currentFrame;

    float frameTime = 0.1f;     
    float timer = 0.f;          

public:
    Action(const std::vector<std::string>& paths, sf::Vector2i _frameCount)
        : frameCount(_frameCount), currentFrame(0, 0)
    {
        loadSprites(paths);
        updateTextureRect();
    }

    void loadSprites(const std::vector<std::string>& paths) {
        textures.resize(paths.size());
        sprites.resize(paths.size());

        for (size_t i = 0; i < paths.size(); ++i) {
            if (!textures[i].loadFromFile(paths[i])) {
                throw std::runtime_error("Failed to load texture: " + paths[i]);
            }
            sprites[i].setTexture(textures[i]);
        }
        frameWidth = textures[0].getSize().x / frameCount.x;
        frameHeight = textures[0].getSize().y / frameCount.y;
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
        

        sf::IntRect rect(currentFrame.x * frameWidth, currentFrame.y * frameHeight, frameWidth, frameHeight);
        for (auto& sprite : sprites) {
            sprite.setTextureRect(rect);
        }
    }

    void setPosition(float x, float y) {
        for (auto& sprite : sprites) {
            sprite.setPosition(x, y);
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

   

    void faceLeft() {
        for (auto& sprite : sprites) {
            sprite.setScale(-1.f, 1.f);
            sprite.setOrigin(sprite.getGlobalBounds().width, 0.f);
        }   
    }

    void faceRight() {
        for (auto& sprite : sprites) {
            sprite.setScale(1.f, 1.f);
            sprite.setOrigin(0.f, 0.f);
        }
    }
};

enum class HeroState { Idle, Walking };

class Hero : public sf::Transformable {
private:
    sf::Vector2f velocity;
    float speed = 100.f;

    std::map<HeroState, Action*> actions;
    HeroState currentState = HeroState::Idle;

public:
    Hero() = default;

    void initActions() {
        // WALKING
        std::vector<std::string> walkingPaths = {
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/base_walk_strip8.png",
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/spikeyhair_walk_strip8.png",
            "assets/Sunnyside_World_Assets/Characters/Human/WALKING/tools_walk_strip8.png"
        };
        Action* walking = new Action(walkingPaths, sf::Vector2i(8, 1));
        actions[HeroState::Walking] = walking;

        // IDLE
        std::vector<std::string> idlePaths = {
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/base_idle_strip9.png",
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/spikeyhair_idle_strip9.png",
            "assets/Sunnyside_World_Assets/Characters/Human/IDLE/tools_idle_strip9.png"
        };
        Action* idle = new Action(idlePaths, sf::Vector2i(9, 1));
        actions[HeroState::Idle] = idle;

        currentState = HeroState::Idle;
    }

    void update(float dt, const sf::Vector2f& movement) {
        // Actualiza estado
        currentState = (movement != sf::Vector2f(0.f, 0.f)) ? HeroState::Walking : HeroState::Idle;

        // Actualiza posición y animación
        actions[currentState]->setPosition(getPosition().x, getPosition().y);
        actions[currentState]->update(dt);

        // Dirección
        if (velocity.x < 0)
            actions[currentState]->faceLeft();
        else if (velocity.x > 0)
            actions[currentState]->faceRight();
    }

    void move(const sf::Vector2f& direction, float dt) {
        velocity = direction * speed;
        sf::Transformable::move(velocity * dt);
    }

    void draw(sf::RenderWindow& window) {
        actions[currentState]->draw(window);
    }

    ~Hero() {
        for (auto& [state, action] : actions) {
            delete action;
        }
    }
};