#pragma once

#include "Chunk.h"

#include <map>
#include <optional>
#include <memory>

class Game;

class World
{
    public:
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

    protected:

    private:
        Game* game;
        int seed;

        std::map<int64_t, std::unique_ptr<Chunk>> chunks;
};
