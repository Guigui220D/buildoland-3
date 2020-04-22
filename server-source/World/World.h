#pragma once

#include "../Utils/Utils.h"
#include "../../common-source/Utils/TileReference.h"
#include "../../common-source/Networking/ECCPacket.h"

#include <unordered_map>
#include <memory>

#include <SFML/System/Clock.hpp>

class Server;
class Block;
class Ground;
class Chunk;

class GameBlocks;
class GameGrounds;
class ItemsRegister;
class Generator;
class EntitiesManager;

class World
{
    public:
        World(Server& server);
        World(Server& server, int seed);
        virtual ~World();

        void init();

        /**
        * Sends a packet to all users that are "subscribed" to the chunk
        * Subscribed means their player is close enough to the chunk to have it loaded
        * @param packet : the packet to send
        * @param chunk : the chunk we want to send to the subscribers of
        */
        void sendToSubscribers(ECCPacket& packet, sf::Vector2i chunk) const;

        /**
        * Sends a packet to all users that are "subscribed" to chunk_a, BUT NOT to chunk_b
        * @param packet : the packet to send
        * @param chunk_a, chunk_b : the chunks
        */
        void sendToSubscribersWithException(ECCPacket& packet, sf::Vector2i chunk_a, sf::Vector2i chunk_b) const;

        EntitiesManager& getEntityManager() { return *entities; }

        inline Server& getServer() const { return server; }

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

        uint16_t getBlockId(sf::Vector2i pos);
        uint16_t getGroundId(sf::Vector2i pos);

        const Block* getBlockPtr(sf::Vector2i pos);
        const Ground* getGroundPtr(sf::Vector2i pos);

        inline TileReference getTile(sf::Vector2i pos) { return TileReference(pos, *this); }

        void setBlock(sf::Vector2i pos, Block const * block);
        void setGround(sf::Vector2i pos, Ground const * ground);

        /**
         * Calculate the position of the chunk that the block is in
         * @param block_pos : the position of the block
         * @return The position of the chunk
         */
        static sf::Vector2i getChunkPosFromBlockPos(sf::Vector2i block_pos);
        /**
         * Calculate the position of the block inside the chunk it is in
         * @param block_pos : the position of the block
         * @return The position of the block inside its chunk
         */
        static sf::Vector2i getBlockPosInChunk(sf::Vector2i block_pos);

        inline const GameBlocks& getBlocksManager() const { return game_blocks_manager; }
        inline const GameGrounds& getGroundsManager() const { return game_grounds_manager; }

        inline Generator* getGenerator() const { return generator; };

        inline size_t getChunksCount() const { return chunks.size(); };
        inline std::unordered_map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksBegin() const { return chunks.cbegin(); }
        inline std::unordered_map<uint64_t, std::unique_ptr<Chunk>>::const_iterator
            getChunksEnd() const { return chunks.cend(); }

        void updateTileEntities(float delta_time);

        void unloadOldChunks();

    protected:
        //Entities
        std::unique_ptr<EntitiesManager> entities;

        Generator* generator;

    private:
        Server& server;
        const GameBlocks& game_blocks_manager;
        const GameGrounds& game_grounds_manager;

        std::unordered_map<uint64_t, std::unique_ptr<Chunk>> chunks;

        sf::Clock last_unload_iteration;
};
