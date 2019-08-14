#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/Arr2D.h"

#include "../Ground/GameGrounds.h"

class Game;

class Chunk
{
    public:
        static const size_t CHUNK_SIZE;

        Chunk(Game* game, sf::Vector2i pos);
        ~Chunk();

        inline Game* getGame() const { return game; }

        inline sf::Vector2i getPos() const { return pos; }

        inline bool isReady() const { return ready; }

        inline sf::VertexArray& getGroundVertexArray() const
            { if (!vertices_ready) generateVertices(); return ground_vertices; }
        inline sf::VertexArray& getGroundDetailsVertexArray() const
            { if (!vertices_ready) generateVertices(); return ground_detail_vertices; }
        inline sf::VertexArray& getBlockSidesVertexArray() const
            { if (!vertices_ready) generateVertices(); return block_side_vertices; }
        inline sf::VertexArray& getBlockTopsVertexArray() const
            { if (!vertices_ready) generateVertices(); return block_top_vertices; }

    private:
        bool ready = false;

        Arr2D<unsigned short> blocks, grounds;
        const sf::Vector2i pos;

        mutable sf::VertexArray ground_vertices, ground_detail_vertices, block_side_vertices, block_top_vertices;
        mutable bool vertices_ready = false, ground_vertices_pos_ready = false;

        void generateVertices() const;

        void generateGroundVertices() const;
        void generateGroundDetailVertices() const;
        void generateBlockSideVertices() const;
        void generateBlockTopVertices() const;

        Game* game;
};
