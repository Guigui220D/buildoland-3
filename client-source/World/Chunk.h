#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <array>
#include <cassert>
#include <atomic>

#include "../../common-source/Utils/Log.h"

class Game;
class Block;
class Ground;
class World;
class TileEntity;

class Chunk
{
    friend class TileEntity;

public:
    static const int CHUNK_SIZE;

    static inline size_t getChunkDataSize() { return CHUNK_SIZE * CHUNK_SIZE * 4; }

    Chunk(World& world, sf::Vector2i pos, const char *chunk_data, unsigned chunk_data_size, bool& success);
    /**
        This is only for the dummy chunk
    */
    Chunk(World& world);
    ~Chunk();

    inline Game& getGame() const { return game; }
    inline World& getWorld() const { return world; }

    inline sf::Vector2i getPos() const { return pos; }

    inline bool isReady() const { return ready; }

    inline uint16_t getBlockId(int x, int y) const
    {
        assert(x >= 0); assert(y >= 0); assert(x < CHUNK_SIZE); assert(y < CHUNK_SIZE);
        return blocks[y*CHUNK_SIZE + x];
    }

    inline uint16_t getGroundId(int x, int y) const
    {
        assert(x >= 0); assert(y >= 0); assert(x < CHUNK_SIZE); assert(y < CHUNK_SIZE);
        return grounds[y*CHUNK_SIZE + x];
    }

    const Block* getBlock(int x, int y) const;
    const Ground* getGround(int x, int y) const;

    inline uint16_t getBlockId(sf::Vector2i pos) const { return getBlockId(pos.x, pos.y); }
    inline uint16_t getGroundId(sf::Vector2i pos) const { return getGroundId(pos.x, pos.y); }

    inline const Block* getBlock(sf::Vector2i pos) const { return getBlock(pos.x, pos.y); }
    inline const Ground* getGround(sf::Vector2i pos) const { return getGround(pos.x, pos.y); }

    void setBlock(int x, int y, const Block* block);
    void setGround(int x, int y, const Ground* ground);

    inline void setBlock(sf::Vector2i pos, const Block* block) { setBlock(pos.x, pos.y, block); }
    inline void setGround(sf::Vector2i pos, const Ground* ground) { setGround(pos.x, pos.y, ground); }

    /**
         * Gets the vertex array to draw the ground
         * The texture to be used with that vertex array is "GROUND_TEXTURES"
         * @return The vertex array
         */
    inline sf::VertexArray& getGroundVertexArray() const
        { return ground_vertices[render_vx_id()]; }
    /**
         * Gets the vertex array to draw the details on the ground
         * The texture to be used with that vertex array is "GROUND_DETAILS"
         * @return The vertex array
         */
    inline sf::VertexArray& getGroundDetailsVertexArray(int frame) const
        { return ground_detail_vertices[render_vx_id()].at(frame); }
    /**
         * Gets the vertex array to draw the sides of the blocks
         * The texture to be used with that vertex array is "/=/=/=/=/=/=/"
         * @return The vertex array
         */
    inline sf::VertexArray& getBlockSidesVertexArray() const
        { return block_side_vertices[render_vx_id()]; }
    /**
         * Gets the vertex array to draw the top of the blocks
         * The texture to be used with that vertex array is "/=/=/=/=/=/=/"
         * @return The vertex array
         */
    inline sf::VertexArray& getBlockTopsVertexArray() const
        { return block_top_vertices[render_vx_id()]; }


    /**
         * Tells the chunk that it should redo its vertex arrays
         */
    inline void invalidateVertexArrays() const
        { vertices_ready = false; }

    inline bool vertexArraysOutOfDate() const
        { return !vertices_ready; }
    inline void swapVertexArrays()
        { current_vertex_array_index = 1 - current_vertex_array_index; } // 1 => 0 OR 0 => 1

    /**
         * Generates all the related vertex arrays for chunk rendering
         */
    void generateVertices() const;

    /**
         * Notifies a neighbor chunk that its vertex array should be updated
         * @param direction : the direction of the chunk (0, 1, 2, 3)
         */
    void notifyChunk(int direction) const;

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

    inline int render_vx_id() const
        { return current_vertex_array_index; }
    inline int gen_vx_id() const
        { return 1 - current_vertex_array_index; }

private:
    bool ready = false;

    std::vector<uint16_t> blocks;
    std::vector<uint16_t> grounds;
    std::vector<TileEntity*> tile_entities;
    const sf::Vector2i pos;

    // some sort of double-swapping technique is used to
    mutable sf::VertexArray ground_vertices[2], block_side_vertices[2], block_top_vertices[2];
    mutable std::array<sf::VertexArray, 4> ground_detail_vertices[2];
    mutable int current_vertex_array_index = 0; // only modified by the async rendered thread; use a mutex in the draw() part so the async renderer doesn't do a swap when drawing

    mutable std::atomic<bool> vertices_ready;
    mutable bool ground_vertices_pos_ready = false;

    void generateGroundVertices() const;
    void generateGroundDetailVertices() const;
    void generateBlockSideVertices() const;
    void generateBlockTopVertices() const;

    Game& game;
    World& world;
};
