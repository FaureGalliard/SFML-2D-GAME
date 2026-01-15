#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class Camera {
public:
    Camera();

    void update(const sf::Vector2f& targetPos, float dt);


    void setSize(const sf::Vector2f& size);

    sf::View getView() const;


    sf::FloatRect getBounds() const;

    void setPosition(const sf::Vector2f& pos);

private:
    sf::View view;
    sf::Vector2f position;
    float smoothSpeed = 5.0f; // Velocidad de interpolación
};