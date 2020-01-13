#pragma once

#include "Chunk.h"

#include "../Utils/Utils.h"
#include "EntitiesManager.h"

#include <map>
#include <memory>

class Server;
class Block;
class Ground;

class GameBlocks;
class GameGrounds;

class World
{
    public:
        World(Server* server);
        World(Server* server, int seed);
        virtual ~World();

        /**
        * Sends a packet to all users that are "subscribed" to the chunk
        * Subscribed means their player is close enough to the chunk to have it loaded
        * @param packet : the packet to send
        * @param chunk : the chunk we want to send to the subscribers of
        */
        void sendToSubscribers(sf::Packet& packet, sf::Vector2i chunk) const;

        /**
        * Sends a packet to all users that are "subscribed" to chunk_a, BUT NOT to chunk_b
        * @param packet : the packet to send
        * @param chunk_a, chunk_b : the chunks
        */
        void sendToSubscribersWithException(sf::Packet& packet, sf::Vector2i chunk_a, sf::Vector2i chunk_b) const;

        EntitiesManager& getEntityManager() { return entities; }

        /**
         * Gets the game that own this world
         * @return A pointer to the game
         */
        inline Server* getServer() const { return server; }
        /**
         * Gets the seed of the world
         * @return The seed
         */
        inline int getSeed() const { return seed; }

        /**
         * Gets a const reference to a chunk with its position
         * An exception will be thrown if the chunk doesn't exist
         * @param pos : the position of the chunk
         * @return The const reference to the chunk
         */
        const Chunk& getChunkConst(sf::Vector2i pos) const;
        /**
         * Gets a reference to a chunk with its position
         * The chunk will be generated if needed
         * @param pos : the position of the chunk
         * @return The reference to the chunk
         */
        Chunk& getChunk(sf::Vector2i pos);
        /**
         * To check whether a chunk is loaded or not
         * @param pos : the position of the chunk
         * @return True if the chunk exists
         */
        inline bool isChunkLoaded(sf::Vector2i pos) const { return chunks.find(utils::combine(pos.x, pos.y)) != chunks.end(); }

        /**
         * Gets a block id from its position
         * @param pos : the position of the block
         * @return The id of the block
         */
        uint16_t getBlockId(sf::Vector2i pos);
        /**
         * Gets a ground id from its position
         * @param pos : the position of the ground
         * @return The id of the ground
         */
        uint16_t getGroundId(sf::Vector2i pos);

        /**
         * Gets a block from its position
         * @param pos : the position of the block
         * @return The pointer to the block
         */
        const Block* getBlock(sf::Vector2i pos);
        /**
         * Gets a ground from its position
         * @param pos : the position of the ground
         * @return The pointer to the ground
         */
        const Ground* getGround(sf::Vector2i pos);
        /**
         * Sets a block at a position with its id
         * @param pos : the position of the block to set
         * @param id : the id of the block
         */
        void setBlock(sf::Vector2i pos, uint16_t id);
        /**
         * Sets a block at a position with its id
         * @param pos : the position of the block to set
         * @param id : the id of the block
         */
        void setGround(sf::Vector2i pos, uint16_t id);

        /**
         * Calculate the position of the chunk that the block is in
         * @param block_pos : the position of the block
         * @return The position of the chunk
         */
        static inline sf::Vector2i getChunkPosFromBlockPos(sf::Vector2i block_pos)
        {
            if (block_pos.x < -1)
                block_pos.x++;
            if (block_pos.y < -1)
                block_pos.y++;

            sf::Vector2i result(block_pos.x / Chunk::CHUNK_SIZE, block_pos.y / Chunk::CHUNK_SIZE);

            if (block_pos.x < 0)
                result.x--;
            if (block_pos.y < 0)
                result.y--;

            return result;
        }
        /**
         * Calculate the position of the block inside the chunk it is in
         * @param block_pos : the position of the block
         * @return The position of the block inside its chunk
         */
        static inline sf::Vector2i getBlockPosInChunk(sf::Vector2i block_pos)
        {
            //sf::Vector2i chunk_pos = getChunkPosFromBlockPos(block_pos);
            sf::Vector2i result(block_pos.x % Chunk::CHUNK_SIZE, block_pos.y % Chunk::CHUNK_SIZE);

            if (result.x < 0)
                result.x += Chunk::CHUNK_SIZE;
            if (result.y < 0)
                result.y += Chunk::CHUNK_SIZE;

            assert(result.x >= 0);
            assert(result.y >= 0);
            assert(result.x < Chunk::CHUNK_SIZE);
            assert(result.y < Chunk::CHUNK_SIZE);

            return result;
        }

        /**
         * Get a reference to the blockManager of the game
         * @return A reference to the blockManager
         */
        inline const GameBlocks& getBlocksManager() const { return game_blocks_manager; }
        /**
         * Get a reference to the groundManager of the game
         * @return A reference to the groundsManager
         */
        inline const GameGrounds& getGroundsManager() const { return game_grounds_manager; }

        inline size_t getChunksCount() const { return chunks.size(); };
        inline std::map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksBegin() const { return chunks.cbegin(); }
        inline std::map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksEnd() const { return chunks.cend(); }

    protected:
        //Entities
        EntitiesManager entities;

    private:
        Server* server;
        const GameBlocks& game_blocks_manager;
        const GameGrounds& game_grounds_manager;
        int seed;

        std::map<uint64_t, std::unique_ptr<Chunk>> chunks;
};
