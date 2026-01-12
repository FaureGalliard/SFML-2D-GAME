#pragma once
#include <SFML/Graphics.hpp>

class Tileset {

public:
    bool load(const std::string& path);
    sf::IntRect get(int tx,int ty) const;
    const sf::Texture& texture() const;
    static constexpr int TILE_SIZE = 16;

private:
    sf::Texture m_texture;


};


