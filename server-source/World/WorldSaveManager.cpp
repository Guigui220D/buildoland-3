#include "WorldSaveManager.h"

#include "../../common-source/Utils/log.h"
#include "../Utils/Base64.h"

#include "Chunk.h"
#include "Generator.h"
#include "../../common-source/Entities/Entity.h"

#include "../../external/json/Json.hpp"

#include <fstream>
#include <iomanip>
#include <cstring>

const size_t WorldSaveManager::LAYER_SIZE = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * sizeof(uint16_t);

WorldSaveManager::WorldSaveManager(std::string where_to_save, World& world, Generator& generator) :
    world(world),
    saving_thread(&WorldSaveManager::thread_loop, this),
    save_dir_path(where_to_save),
    stop_thread(false),
    generator(generator)
{
    std::ifstream world_info(fmt::format("{}/world_settings.json", where_to_save));

    int seed = std::rand();

    if (world_info.is_open())
    {
        nlohmann::json json;

        world_info >> json;

        if (json.is_object())
        {
            auto jseed = json["seed"];
            if (jseed.is_number())
                seed = jseed.get<int>();
        }
    }

    generator.seed = seed;

    log(INFO, "Starting chunk saving thread\n");
    saving_thread.launch();
}

WorldSaveManager::~WorldSaveManager()
{
    stop_thread = true;
    log(INFO, "Stopping chunk saving thread\n");

    nlohmann::json json;
    json["seed"] = generator.seed;

    std::ofstream o(fmt::format("{}/world_settings.json", save_dir_path));

    if (o.is_open())
        o << json;

    o.close();

    saving_thread.wait();
}

void WorldSaveManager::addChunkToSave(ChunkWithEntities* chunk_to_save)
{
    save_queue_mutex.lock();
    chunks_to_save.push(chunk_to_save);
    save_queue_mutex.unlock();
}

sf::Vector2i WorldSaveManager::getRegionFromChunk(sf::Vector2i chunk) const
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

void WorldSaveManager::requestChunk(sf::Vector2i chunk)
{
    load_queue_mutex.lock();
        log(INFO, "Requesting chunk ({}; {})!\n", chunk.x, chunk.y);
        chunks_to_load.push(chunk);
    load_queue_mutex.unlock();
}

ChunkWithEntities* WorldSaveManager::popLoadedChunk()
{
    sf::Lock lock(loaded_queue_mutex);

    if (loaded_chunks.empty())
        return nullptr;

    ChunkWithEntities* cwe = loaded_chunks.front();
    loaded_chunks.pop();

    log(INFO, "Popped loaded chunk ({}; {})!\n", cwe->chunk_pos.x, cwe->chunk_pos.y);

    return cwe;
}

void WorldSaveManager::thread_loop()
{
    while (1)
    {
        save_queue_mutex.lock();
            bool cts = !chunks_to_save.empty();
        save_queue_mutex.unlock();

        if (stop_thread && !cts)
            break;

        load_queue_mutex.lock();
            bool ctl = !chunks_to_load.empty();
        load_queue_mutex.unlock();

        if (!cts && !ctl)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        if (cts)
        {
            save_queue_mutex.lock();
                ChunkWithEntities* chunk = chunks_to_save.front();
                chunks_to_save.pop();
            save_queue_mutex.unlock();

            saveChunk(chunk);
        }
        if (ctl && !stop_thread)
        {
            load_queue_mutex.lock();
                sf::Vector2i pos = chunks_to_load.front();
                chunks_to_load.pop();
            load_queue_mutex.unlock();

            ChunkWithEntities* new_chunk = new ChunkWithEntities();

            new_chunk->chunk = new Chunk(world, pos);
            new_chunk->chunk_pos = pos;
            new_chunk->generate_entities = true;

            sf::Vector2i region = getRegionFromChunk(pos);
            std::string filename = fmt::format("{}/region_{}_{}.json", save_dir_path, region.x, region.y);

            bool generate_tiles = true;
            bool generate_entities = true;

            //TODO : try to load from file

            std::ifstream i(filename);

            if (i.is_open())
            {
                nlohmann::json json;

                i >> json;

                if (json.is_object())
                {
                    std::string chunk_json = fmt::format("chunk_{}_{}", pos.x, pos.y);

                    auto json_i = json[chunk_json];

                    if (json_i.is_object())
                    {
                        log(INFO, "Loading chunk ({}; {}) from region file \"{}\".\n", pos.x, pos.y, filename);

                        auto block_layer = json_i["block_layer"];
                        auto ground_layer = json_i["ground_layer"];

                        bool block_valid = block_layer.is_string() /*&& check size*/;
                        bool ground_valid = ground_layer.is_string() /*&& check size*/;

                        if (block_valid && ground_valid)
                        {
                            std::string block_bytes = base64_decode(block_layer.get<std::string>());
                            std::string ground_bytes = base64_decode(ground_layer.get<std::string>());

                            if (block_bytes.size() == LAYER_SIZE && ground_bytes.size() == LAYER_SIZE)
                            {
                                std::memcpy(new_chunk->chunk->blocks.data(), block_bytes.data(), block_bytes.size());
                                std::memcpy(new_chunk->chunk->grounds.data(), ground_bytes.data(), ground_bytes.size());

                                generate_tiles = false;
                            }
                        }
                    }
                }

                i.close();
            }

            if (generate_tiles)
            {
                if (generate_entities)
                {
                    generator.generateChunk(new_chunk->chunk, new_chunk->entities);
                }
                else
                {
                    std::vector<Entity*> entity_trashcan;
                    generator.generateChunk(new_chunk->chunk, entity_trashcan); //Litterally asking the generator to make entities and destroy them after
                    for (Entity* e : entity_trashcan)
                        delete e;
                }
            }

            new_chunk->chunk->modified = false;
            new_chunk->chunk->ready = true;

            new_chunk->chunk->prepareTileEntities();

            log(INFO, "Loaded chunk ({}; {}).\n", pos.x, pos.x);

            loaded_queue_mutex.lock();
                loaded_chunks.push(new_chunk);
            loaded_queue_mutex.unlock();
        }
    }
}

void WorldSaveManager::saveChunk(ChunkWithEntities* cwe)
{
    sf::Vector2i region = getRegionFromChunk(cwe->chunk_pos);

    std::string filename = fmt::format("{}/region_{}_{}.json", save_dir_path, region.x, region.y);
    std::string chunk_json = fmt::format("chunk_{}_{}", cwe->chunk_pos.x, cwe->chunk_pos.y);

    log(INFO, "Saving chunk ({}; {}) in region file \"{}\".\n", cwe->chunk_pos.x, cwe->chunk_pos.y, filename);

    nlohmann::json json;

    {
        std::ifstream i(filename);

        if (i.is_open())
            i >> json;
        i.close();
    }

    if(cwe->chunk)
    {
        log(INFO, "    Chunk has tile data.\n");

        json[chunk_json]["block_layer"] = base64_encode((unsigned char*)cwe->chunk->blocks.data(), LAYER_SIZE);
        json[chunk_json]["ground_layer"] = base64_encode((unsigned char*)cwe->chunk->grounds.data(), LAYER_SIZE);

        delete cwe->chunk;
    }

    if (!cwe->entities.empty())
    {
        log(INFO, "    Chunk has {} entities.\n", cwe->entities.size());

        int i = 0;

        while (!cwe->entities.empty())
        {
            json[chunk_json]["entities"][i++] = "test";
            delete *cwe->entities.begin();
            cwe->entities.erase(cwe->entities.begin());
        }
    }

    if (!json.is_null())
    {
        std::ofstream o(filename);
        o << std::setw(4) << json << std::endl;
        o.close();
    }

    delete cwe;
}
