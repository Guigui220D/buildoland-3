#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <array>

#include "../Utils/Arr2D.h"

#include "../Ground/GameGrounds.h"

class Game;
class Block;
class Ground;

class Chunk
{
    public:
        static const int CHUNK_SIZE;

        static inline size_t getChunkDataSize() { return CHUNK_SIZE * CHUNK_SIZE * 4; }

        Chunk(World* world, sf::Vector2i pos, sf::Packet packet, bool& success);
        ~Chunk();

        inline Game* getGame() const { return game; }

        inline sf::Vector2i getPos() const { return pos; }

        inline bool isReady() const { return ready; }

        inline uint16_t getBlockId(int x, int y) const
        {
            assert(x >= 0); assert(y >= 0); assert(x < CHUNK_SIZE); assert(y < CHUNK_SIZE);
            return blocks.get(x, y);
        }

        inline uint16_t getGroundId(int x, int y) const
        {
            assert(x >= 0); assert(y >= 0); assert(x < CHUNK_SIZE); assert(y < CHUNK_SIZE);

            return grounds.get(x, y);
        }

        const Block* getBlock(int x, int y) const;
        const Ground* getGround(int x, int y) const;

        inline uint16_t getBlockId(sf::Vector2i pos) const { return getBlockId(pos.x, pos.y); }
        inline uint16_t getGroundId(sf::Vector2i pos) const { return getGroundId(pos.x, pos.y); }

        inline const Block* getBlock(sf::Vector2i pos) const { return getBlock(pos.x, pos.y); }
        inline const Ground* getGround(sf::Vector2i pos) const { return getGround(pos.x, pos.y); }

        void setBlock(int x, int y, uint16_t id);
        void setGround(int x, int y, uint16_t id);

        void setBlock(int x, int y, const Block* block);
        void setGround(int x, int y, const Ground* ground);

        inline void setBlock(sf::Vector2i pos, uint16_t id) { setBlock(pos.x, pos.y, id); }
        inline void setGround(sf::Vector2i pos, uint16_t id) { setGround(pos.x, pos.y, id); }

        inline void setBlock(sf::Vector2i pos, const Block* block) { setBlock(pos.x, pos.y, block); }
        inline void setGround(sf::Vector2i pos, const Ground* ground) { setGround(pos.x, pos.y, ground); }

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

        /**
         * Tells the chunk that it should redo its vertex arrays
         */
        inline void mustRedoVertexArrays() const { vertices_ready = false; }
        /**
         * Notifies a neighbor chunk that its vertex array should be updated
         * @param direction : the direction of the chunk (0, 1, 2, 3)
         */
        void notifyChunk(int direction) const;

        //For testing
        /**
         * THIS IS A TESTING FUNCTION
         */
        void regenerate();

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

        bool to_be_removed = false;

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
