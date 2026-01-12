//
// Created by angel on 8/01/2026.
//

#include "Tileset.h"
#include <cassert>
bool Tileset::load(const std::string& path) {

    return m_texture.loadFromFile(path);
}

sf::IntRect Tileset::get(int tx, int ty, int w, int h) const {
    assert(tx >= 0 && ty >= 0 && w > 0 && h > 0);
    return {
        tx * TILE_SIZE,
        ty * TILE_SIZE,
        w * TILE_SIZE,
        h * TILE_SIZE
    };
}

const sf::Texture& Tileset::texture() const {

    return m_texture;
}