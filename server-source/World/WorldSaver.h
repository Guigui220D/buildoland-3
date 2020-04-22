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

class WorldSaver
{
    public:
        using EntitiesGroup = std::shared_ptr<std::vector<Entity*>>;
        using ChunkWithEntities = std::pair<std::pair<sf::Vector2i, Chunk*>, EntitiesGroup>;

        WorldSaver(std::string where_to_save);
        ~WorldSaver();

        void addChunkToSave(ChunkWithEntities chunk_to_save);

    private:
        std::queue<ChunkWithEntities> chunks_to_save;

        sf::Thread saving_thread;
        sf::Mutex queue_mutex;

        std::string save_dir_path;

        std::atomic_bool stop_thread;

        void saving_loop();
};
