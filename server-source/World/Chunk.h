#pragma once

#include <SFML/Network.hpp>

#include <cassert>

#include "../../common-source/Entities/TileEntity.h"

//#include "../Ground/GameGrounds.h"

class Server;
class World;
class Block;
class Ground;

class Chunk
{
    friend class TileEntity;

    public:
        static const int CHUNK_SIZE;

        static inline size_t getChunkDataSize() { return CHUNK_SIZE * CHUNK_SIZE * 4; }

        Chunk(World& world, sf::Vector2i pos);
        ~Chunk();

        inline Server& getServer() const { return server; }

        inline sf::Vector2i getPos() const { return pos; }

        inline bool isReady() const { return ready; }

        /**
         * Generates a packet containing the chunk's information
         */
        ECCPacket& getPacket();

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


        inline sf::Vector2f getCenter() const { return sf::Vector2f(pos.x * CHUNK_SIZE + .5f * CHUNK_SIZE - .5f, pos.y * CHUNK_SIZE + .5f * CHUNK_SIZE - .5f); }

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

        std::vector<uint16_t> blocks;
        std::vector<uint16_t> grounds;
        std::vector<TileEntity*> tile_entities;
        const sf::Vector2i pos;

        Server& server;
        World& world;

        std::unique_ptr<ECCPacket> packet;
        bool packet_ready;

        void generatePacket();
};
