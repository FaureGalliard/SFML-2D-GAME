//
// Created by angel on 8/01/2026.
//

#include "Tileset.h"

bool Tileset::load(const std::string& path) {

    return m_texture.loadFromFile(path);
}

sf::IntRect Tileset::get(int tx,int ty) const {

    return{

        tx*TILE_SIZE,
        ty*TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE

    };

}

const sf::Texture& Tileset::texture() const {

    return m_texture;
}