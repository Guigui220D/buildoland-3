#include "WorldSaver.h"

#include "../../common-source/Utils/log.h"

#include "Chunk.h"

WorldSaver::WorldSaver(std::string where_to_save) :
    saving_thread(&WorldSaver::saving_loop, this),
    save_dir_path(where_to_save),
    stop_thread(false)
{
    log(INFO, "Starting chunk saving thread\n");
    saving_thread.launch();
}

WorldSaver::~WorldSaver()
{
    stop_thread = true;
    log(INFO, "Stopping chunk saving thread\n");
    saving_thread.wait();
}

void WorldSaver::addChunkToSave(Chunk* chunk_to_save)
{
    queue_mutex.lock();
    chunks_to_save.push(chunk_to_save);
    queue_mutex.unlock();
}

void WorldSaver::saving_loop()
{
    while (!stop_thread)
    {
        //log(INFO, "Bou\n");
        if (chunks_to_save.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            queue_mutex.lock();

            Chunk* chunk = chunks_to_save.front();
            chunks_to_save.pop();

            log(INFO, "Saving chunk {}; {} (NOT IMPLEMENTED)\n", chunk->getPos().x, chunk->getPos().y);

            delete chunk;

            queue_mutex.unlock();
        }
    }
}
