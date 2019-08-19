#pragma once

#include "Chunk.h"

#include <map>
#include <optional>
#include <memory>

class Game;
class Block;
class Ground;

class GameBlocks;
class GameGrounds;

class World
{
    public:
        static unsigned int RENDER_DISTANCE;

        World(Game* game);
        World(Game* game, int seed);
        virtual ~World();

        /**
         * Gets the game that own this world
         * @return A pointer to the game
         */
        inline Game* getGame() const { return game; }
        /**
         * Gets the seed of the world
         * @return The seed
         */
        inline int getSeed() const { return seed; }

        /**
         * Loads close chunks and unload other chunks
         * @param center : the position of the player / the camera
         */
        void updateLoadedChunk(sf::Vector2f center);

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
        uint16_t getBlock(sf::Vector2i pos, bool load);
        /**
         * Gets a ground id from its position
         * @param pos : the position of the ground
         * @return The id of the ground
         */
        uint16_t getGround(sf::Vector2i pos, bool load);

        /**
         * Gets a block from its position
         * @param pos : the position of the block
         * @return The pointer to the block
         */
        const Block* getBlockPtr(sf::Vector2i pos, bool load);
        /**
         * Gets a ground from its position
         * @param pos : the position of the ground
         * @return The pointer to the ground
         */
        const Ground* getGroundPtr(sf::Vector2i pos, bool load);

        /**
         * Calculate the position of the chunk that the block is in
         * @param block_pos : the position of the block
         * @return The position of the chunk
         */
        static inline sf::Vector2i getChunkPosFromBlockPos(sf::Vector2i block_pos) { return sf::Vector2i(block_pos.x / Chunk::CHUNK_SIZE, block_pos.y / Chunk::CHUNK_SIZE); }
        /**
         * Calculate the position of the block inside the chunk it is in
         * @param block_pos : the position of the block
         * @return The position of the block inside its chunk
         */
        static inline sf::Vector2i getBlockPosInChunk(sf::Vector2i block_pos)
        {
            sf::Vector2i chunk_pos = getChunkPosFromBlockPos(block_pos);
            return sf::Vector2i(block_pos.x - chunk_pos.x * Chunk::CHUNK_SIZE, block_pos.y - chunk_pos.y * Chunk::CHUNK_SIZE);
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
        inline std::map<int64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksBegin() const { return chunks.cbegin(); }
        inline std::map<int64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksEnd() const { return chunks.cend(); }

    protected:

    private:
        Game* game;
        const GameBlocks& gameBlocksManager;
        const GameGrounds& gameGroundsManager;
        int seed;

        std::map<int64_t, std::unique_ptr<Chunk>> chunks;
};
