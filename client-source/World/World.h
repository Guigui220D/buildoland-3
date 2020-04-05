#pragma once

#include "Chunk.h"
#include "EntitiesManager.h"
#include "../Utils/Utils.h"

#include <unordered_map>
#include <memory>

class GameState;
class Block;
class Ground;

class GameBlocks;
class GameGrounds;

class World
{
    public:
        static unsigned int RENDER_DISTANCE;

        World(GameState& state_game);
        World(GameState& state_game, int seed);
        virtual ~World();

        EntitiesManager& getEntityManager() { return entities; }

        /**
         * Gets the state that own this world
         * @return A pointer to the state game
         */
        inline GameState& getState() const { return state_game; }
        /**
         * Gets the game that own this world
         * @return A pointer to the game
         */
        inline Game& getGame() const { return game; }
        /**
         * Gets the seed of the world
         * @return The seed
         */
        inline int getSeed() const { return seed; }

        /**
         * Loads close chunks and unload other chunks
         */
        void updateLoadedChunk(float delta_time);

        /**
         * Adds a chunk from a received packet
         * The chunk won't be added if the data was invalid
         * This function is thread safe
         * @param packet : the packet to unpack
         * @return True if the chunk was added
         */
        bool addChunk(sf::Vector2i pos, const char *chunk_data, unsigned chunk_size);

        /**
         * Gets a reference to a chunk with its position
         * An exception will be thrown if the chunk doesn't exist
         * @param pos : the position of the chunk
         * @return The const reference to the chunk
         */
        Chunk& getChunk(sf::Vector2i pos);

        /**
         * TEMPORARY
         * Gets a const reference to a chunk with its position
         * An exception will be thrown if the chunk doesn't exist
         * @param pos : the position of the chunk
         * @return The const reference to the chunk
         */
        const Chunk& getChunkConst(sf::Vector2i pos) const;
        /**
         * Requests a chunk to the server
         * @param pos : the position of the chunk
         */
        void requestChunk(sf::Vector2i pos);
        /**
         * To check whether a chunk is loaded or not
         * @param pos : the position of the chunk
         * @return True if the chunk exists
         */
        inline bool isChunkLoaded(sf::Vector2i pos) const { return chunks.find(utils::combine(pos.x, pos.y)) != chunks.end(); }
        /**
         * Loads nearby chunks and unload further chunks
         * @param center : the chunk the player is in
         */
        void updateChunks(sf::Vector2i center);

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
        inline std::unordered_map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksBegin() const { return chunks.cbegin(); }
        inline std::unordered_map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksEnd() const { return chunks.cend(); }

        inline void setSeed(int new_seed) { seed = new_seed; }

    protected:
        //Entities
        EntitiesManager entities;

    private:
        GameState& state_game;
        Game& game;
        const GameBlocks& game_blocks_manager;
        const GameGrounds& game_grounds_manager;
        int seed;

        std::unordered_map<uint64_t, std::unique_ptr<Chunk>> chunks;
        std::vector<Chunk*> chunks_to_add;
        std::unordered_map<uint64_t, sf::Clock> pending_chunk_requests;
        sf::Vector2i player_chunk_pos;
};
