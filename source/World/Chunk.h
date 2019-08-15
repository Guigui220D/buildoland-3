#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/Arr2D.h"

#include "../Ground/GameGrounds.h"

class Game;

class Chunk
{
    public:
        static const size_t CHUNK_SIZE;

        Chunk(World* world, sf::Vector2i pos);
        ~Chunk();

        /**
         * Gets the game that owns this chunk
         * @return The pointer to the game
         */
        inline Game* getGame() const { return game; }

        /**
         * Gets the position of this chunk
         * @return The position of the chunk
         */
        inline sf::Vector2i getPos() const { return pos; }

        /**
         * To know if this chunk is ready
         * @return True if the chunk is ready
         */
        inline bool isReady() const { return ready; }

        /**
         * Gets the vertex array to draw the ground
         * The texture to be used with that vertex array is "GROUND_TEXTURES"
         * @return The vertex array
         */
        inline sf::VertexArray& getGroundVertexArray() const
            { if (!vertices_ready) generateVertices(); return ground_vertices; }
        /**
         * Gets the vertex array to draw the details on the ground
         * The texture to be used with that vertex array is "GROUND_DETAILS"
         * @return The vertex array
         */
        inline sf::VertexArray& getGroundDetailsVertexArray() const
            { if (!vertices_ready) generateVertices(); return ground_detail_vertices; }
        /**
         * Gets the vertex array to draw the sides of the blocks
         * The texture to be used with that vertex array is "/=/=/=/=/=/=/"
         * @return The vertex array
         */
        inline sf::VertexArray& getBlockSidesVertexArray() const
            { if (!vertices_ready) generateVertices(); return block_side_vertices; }
        /**
         * Gets the vertex array to draw the top of the blocks
         * The texture to be used with that vertex array is "/=/=/=/=/=/=/"
         * @return The vertex array
         */
        inline sf::VertexArray& getBlockTopsVertexArray() const
            { if (!vertices_ready) generateVertices(); return block_top_vertices; }

        //For testing
        /**
         * THIS IS A TESTING FUNCTION
         */
        void regenerate();

    private:
        bool ready = false;

        Arr2D<uint16_t> blocks, grounds;
        const sf::Vector2i pos;

        mutable sf::VertexArray ground_vertices, ground_detail_vertices, block_side_vertices, block_top_vertices;
        mutable bool vertices_ready = false, ground_vertices_pos_ready = false;

        void generateVertices() const;

        void generateGroundVertices() const;
        void generateGroundDetailVertices() const;
        void generateBlockSideVertices() const;
        void generateBlockTopVertices() const;

        Game* game;
        World* world;
};
