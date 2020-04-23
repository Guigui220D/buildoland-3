#include "WorldSaver.h"

#include "../../common-source/Utils/log.h"
#include "../Utils/Base64.h"

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

void WorldSaver::addChunkToSave(ChunkWithEntities* chunk_to_save)
{
    queue_mutex.lock();
    chunks_to_save.push(chunk_to_save);
    queue_mutex.unlock();
}

void WorldSaver::saving_loop()
{
    while (!stop_thread || !chunks_to_save.empty())
    {
        if (chunks_to_save.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            queue_mutex.lock();

            ChunkWithEntities* chunk = chunks_to_save.front();
            chunks_to_save.pop();

            log(INFO, "Saving {}; {} (NOT IMPLEMENTED)\n", chunk->chunk_pos.x, chunk->chunk_pos.y);

            if(chunk->chunk)
            {
                log(INFO, "    Saving tile data with it\n");
                delete chunk->chunk;
            }

            log(INFO, "    Saving {} entities with it.\n", chunk->entities.size());

            delete chunk;

            queue_mutex.unlock();
        }
    }
}
