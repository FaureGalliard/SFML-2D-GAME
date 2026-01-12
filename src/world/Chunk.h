#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include "world/Tile.h"

class WorldRenderer;

class Chunk {
public:
        static constexpr int SIZE = 32;
        Chunk(int cx,int cy);

        const sf::VertexArray& getMesh() const;
        sf::Vector2f getWorldPosition() const;

        const Tile& getTile(int x, int y) const;
        int getCX() const;
        int getCY() const;
        bool meshBuilt = false;
        bool isMeshBuilt() const { return meshBuilt; }
        void buildMesh(const WorldRenderer& renderer);


    private:


        void generate();
        void finalizeAutoTiling();
        int cx,cy;
        Tile tiles[SIZE][SIZE];

        sf::VertexArray mesh;

        bool sameType(int x,int y, TileType t) const;
        uint8_t computeMask(int x, int y) const;

};
