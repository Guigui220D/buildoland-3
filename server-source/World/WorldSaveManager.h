#pragma once

#include <queue>
#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <memory>

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>

class Chunk;
class Entity;

struct ChunkWithEntities
{
    sf::Vector2i chunk_pos;
    Chunk* chunk;
    std::vector<Entity*> entities;
    bool generate_entities; //Only useful for loaded chunks and not for saving them
};

class WorldSaveManager
{
    public:
        static const size_t LAYER_SIZE;

        WorldSaveManager(std::string where_to_save);
        ~WorldSaveManager();

        /**
        * Add a chunk and its entities to save to the queue. This functions is thread safe and the queue will be read on the saving thread
        * @param chunks_to_save : The chunk + entities to save
        */
        void addChunkToSave(ChunkWithEntities* chunk_to_save);

        /**
        * Gets the region position of a chunk
        * A region is a group of chunks, region files hold multiple chunks
        * @param chunk : The position of the chunk
        * @return The position of the region
        */
        sf::Vector2i getRegionFromChunk(sf::Vector2i chunk) const;

        /**
        * Requests a chunk to be loaded. It will be loaded in the save/load thread and will be availible through popLoadedChunk()
        * @param chunk : The position of the chunk to load
        */
        void requestChunk(sf::Vector2i chunk);

        /**
        * Pops a loaded chunk with its entities
        * Request a chunk load with requestChunk()
        * If the resulting struct pointer is nullptr, the queue is empty
        * If the resulting chunk is nullptr, it has to be generated.
        * If the struct has generate_entities as true, they should be generated as well
        * The resulting struct needs to be destroyed
        * @return A pointer to a loaded chunk with entities popped (FIFO)
        */
        ChunkWithEntities* popLoadedChunk();

    private:
        std::queue<ChunkWithEntities*> chunks_to_save;
        sf::Mutex save_queue_mutex;

        std::queue<sf::Vector2i> chunks_to_load;
        sf::Mutex load_queue_mutex;

        std::queue<ChunkWithEntities*> loaded_chunks;
        sf::Mutex loaded_queue_mutex;

        sf::Thread saving_thread;

        std::string save_dir_path;

        std::atomic_bool stop_thread;

        void thread_loop();

        void saveChunk(ChunkWithEntities* cwe);
};
