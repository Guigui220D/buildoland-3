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
         * Calculate the position of the chunk that the block is in
         * @param block_pos : the position of the block
         * @return The position of the chunk
         */
        static inline sf::Vector2i getChunkPosFromBlockPos(sf::Vector2i block_pos)
        {
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
                result.x = Chunk::CHUNK_SIZE + result.x;
            if (result.y < 0)
                result.y = Chunk::CHUNK_SIZE + result.y;

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
        inline const GameBlocks& getBlocksManager() const { return gameBlocksManager; }
        /**
         * Get a reference to the groundManager of the game
         * @return A reference to the groundsManager
         */
        inline const GameGrounds& getGroundsManager() const { return gameGroundsManager; }

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
        const GameBlocks& gameBlocksManager;
        const GameGrounds& gameGroundsManager;
        int seed;

        std::map<uint64_t, std::unique_ptr<Chunk>> chunks;
};
