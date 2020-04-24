#include "WorldSaver.h"

#include "../../common-source/Utils/log.h"
#include "../Utils/Base64.h"

#include "Chunk.h"
#include "../../common-source/Entities/Entity.h"

#include "../../external/json/Json.hpp"

#include <fstream>
#include <iomanip>

const size_t WorldSaver::LAYER_SIZE = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * sizeof(uint16_t);

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
            queue_mutex.unlock();

            sf::Vector2i region = getRegionFromChunk(chunk->chunk_pos);

            std::string filename = fmt::format("{}/region_{}_{}.json", save_dir_path, region.x, region.y);
            std::string chunk_json = fmt::format("chunk_{}_{}", chunk->chunk_pos.x, chunk->chunk_pos.y);

            log(INFO, "Saving chunk ({}; {}) in region file \"{}\".\n", chunk->chunk_pos.x, chunk->chunk_pos.y, filename);

            nlohmann::json json;

            {
                std::ifstream i(filename);

                if (i.is_open())
                    i >> json;
                i.close();
            }

            if(chunk->chunk)
            {
                log(INFO, "    Chunk has tile data.\n");

                json[chunk_json]["block_layer"] = base64_encode((unsigned char*)chunk->chunk->blocks.data(), LAYER_SIZE);
                json[chunk_json]["ground_layer"] = base64_encode((unsigned char*)chunk->chunk->grounds.data(), LAYER_SIZE);

                delete chunk->chunk;
            }

            if (!chunk->entities.empty())
            {
                log(INFO, "    Chunk has {} entities.\n", chunk->entities.size());

                int i = 0;

                while (!chunk->entities.empty())
                {
                    json[chunk_json]["entities"][i++] = "test";
                    delete *chunk->entities.begin();
                    chunk->entities.erase(chunk->entities.begin());
                }
            }

            if (!json.is_null())
            {
                std::ofstream o(filename);
                o << std::setw(4) << json << std::endl;
                o.close();
            }

            delete chunk;


        }
    }
}

sf::Vector2i WorldSaver::getRegionFromChunk(sf::Vector2i chunk) const
{
    if (chunk.x < -1)
        chunk.x++;
    if (chunk.y < -1)
        chunk.y++;

    sf::Vector2i result(chunk.x / Chunk::REGION_SIZE, chunk.y / Chunk::REGION_SIZE);

    if (chunk.x < 0)
        result.x--;
    if (chunk.y < 0)
        result.y--;

    return result;
}

