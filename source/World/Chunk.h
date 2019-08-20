#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "../Utils/Arr2D.h"

#include "../Ground/GameGrounds.h"

class Game;
class Block;
class Ground;

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
         * Gets a block id from its position
         * @param x y : the position of the block
         * @return The id of the block
         */
        inline uint16_t getBlockId(int x, int y) const { return blocks.get(x, y); }
        /**
         * Gets a ground id from its position
         * @param x y : the position of the ground
         * @return The id of the ground
         */
        inline uint16_t getGroundId(int x, int y) const { return grounds.get(x, y); }

        /**
         * Gets a block from its position
         * @param x y : the position of the block
         * @return The pointer to the block
         */
        const Block* getBlock(int x, int y) const;
        /**
         * Gets a ground from its position
         * @param x y : the position of the ground
         * @return The pointer to the ground
         */
        const Ground* getGround(int x, int y) const;

        /**
         * Gets a block from its position
         * @param x y : the position of the block
         * @param block : the block to set there
         */
        void setBlock(int x, int y, const Block* block);
        /**
         * Gets a ground from its position
         * @param x y : the position of the ground
         * @param ground : the ground to set there
         */
        void setGround(int x, int y, const Ground* ground);

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
        inline sf::VertexArray& getGroundDetailsVertexArray(int frame) const
            { if (!vertices_ready) generateVertices(); return ground_detail_vertices.at(frame); }
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

        inline void mustRedoVertexArrays() const { vertices_ready = false; }

        //For testing
        /**
         * THIS IS A TESTING FUNCTION
         */
        void regenerate();

        /**
         * Gets the center of this chunk
         * @return The center of this chunk
         */
        inline sf::Vector2f getCenter() const { return sf::Vector2f(.5f * CHUNK_SIZE - .5f, .5f * CHUNK_SIZE - .5f); }

        /**
         * Gets the position of a block of this chunk in the world
         * @param block_pos : The coordinates of the block in this chunk
         * @return The global position
         */
        inline sf::Vector2i getBlockPosInWorld(sf::Vector2i block_pos) const { return sf::Vector2i(block_pos.x + pos.x * CHUNK_SIZE, block_pos.y + pos.y * CHUNK_SIZE); }
        /**
         * Gets the position of a block of this chunk in the world
         * @param x, y : The coordinates of the block in this chunk
         * @return The global position
         */
        inline sf::Vector2i getBlockPosInWorld(int x, int y) const { return sf::Vector2i(x + pos.x * CHUNK_SIZE, y + pos.y * CHUNK_SIZE); }

    private:
        bool ready = false;

        Arr2D<uint16_t> blocks, grounds;
        const sf::Vector2i pos;

        mutable sf::VertexArray ground_vertices, block_side_vertices, block_top_vertices;
        mutable std::array<sf::VertexArray, 4> ground_detail_vertices;

        mutable bool vertices_ready = false, ground_vertices_pos_ready = false;

        void generateVertices() const;

        void generateGroundVertices() const;
        void generateGroundDetailVertices() const;
        void generateBlockSideVertices() const;
        void generateBlockTopVertices() const;

        Game* game;
        World* world;
};
