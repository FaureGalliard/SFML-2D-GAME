#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "core/Config.h"
class Tileset {

public:
    bool load(const std::string& path);
    sf::IntRect get(int tx, int ty, int w = 1, int h = 1) const;
    const sf::Texture& texture() const;

private:
    sf::Texture m_texture;


};


