#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/Arr2D.h"

class Chunk
{
    public:
        static const size_t CHUNK_SIZE;

        Chunk(sf::Vector2i pos);
        ~Chunk();

        inline sf::Vector2i getPos() const { return pos; }

        inline bool isReady() const { return ready; }

        inline sf::VertexArray& getGroundVertexArray()
            { if (!vertices_ready) generateVertices(); return ground_vertices; }
        inline sf::VertexArray& getBlockSidesVertexArray()
            { if (!vertices_ready) generateVertices(); return block_side_vertices; }
        inline sf::VertexArray& getBlockTopsVertexArray()
            { if (!vertices_ready) generateVertices(); return block_top_vertices; }

    private:
        bool ready = false;

        Arr2D<unsigned short> blocks, grounds;
        const sf::Vector2i pos;

        sf::VertexArray ground_vertices, block_side_vertices, block_top_vertices;
        bool vertices_ready = false;
        bool ground_vertices_pos_ready = false;

        void generateVertices();
};
