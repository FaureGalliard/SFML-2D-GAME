#include "Camera.h"
#include <cmath>

Camera::Camera() {
    view.setSize(1280.0f, 720.0f);
    position = {0.0f, 0.0f};
    view.setCenter(position);
}

void Camera::update(const sf::Vector2f& targetPos, float dt) {
    // Interpolación suave hacia el objetivo
    sf::Vector2f diff = targetPos - position;
    position += diff * smoothSpeed * dt;
    
    view.setCenter(position);
}

void Camera::setSize(const sf::Vector2f& size) {
    view.setSize(size);
}

sf::View Camera::getView() const {
    return view;
}

sf::FloatRect Camera::getBounds() const {
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();
    
    return sf::FloatRect(
        center.x - size.x * 0.5f,
        center.y - size.y * 0.5f,
        size.x,
        size.y
    );
}

void Camera::setPosition(const sf::Vector2f& pos) {
    position = pos;
    view.setCenter(position);
}