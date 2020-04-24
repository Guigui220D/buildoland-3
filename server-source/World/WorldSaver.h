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
};

class WorldSaver
{
    public:
        static const size_t LAYER_SIZE;

        WorldSaver(std::string where_to_save);
        ~WorldSaver();

        void addChunkToSave(ChunkWithEntities* chunk_to_save);

        sf::Vector2i getRegionFromChunk(sf::Vector2i chunk) const;

    private:
        std::queue<ChunkWithEntities*> chunks_to_save;

        sf::Thread saving_thread;
        sf::Mutex queue_mutex;

        std::string save_dir_path;

        std::atomic_bool stop_thread;

        void saving_loop();
};
