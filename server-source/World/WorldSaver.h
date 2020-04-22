#pragma once

#include <queue>
#include <string>
#include <atomic>
#include <thread>

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>

class Chunk;

class WorldSaver
{
    public:
        WorldSaver(std::string where_to_save);
        ~WorldSaver();

        void addChunkToSave(Chunk* chunk_to_save);

    private:
        std::queue<Chunk*> chunks_to_save;

        sf::Thread saving_thread;
        sf::Mutex queue_mutex;

        std::string save_dir_path;

        std::atomic_bool stop_thread;

        void saving_loop();
};
