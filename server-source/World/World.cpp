#include "World.h"

#include "../Server/Server.h"

#include "../../common-source/Blocks/Block.h"
#include "../../common-source/Grounds/Ground.h"
#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../server-source/World/Chunk.h"

#include "../../common-source/Networking/NetworkingCodes.h"
#include "../Packets/SetTilePacket.h"

#include "../../common-source/Utils/Log.h"

#include "EntitiesManager.h"
#include "Generators/NaturalGenerator.h"

#include "WorldSaver.h"

World::World(Server& server, WorldSaver& saver) :
    entities(std::make_unique<EntitiesManager>(server)),
    generator(new NaturalGenerator(std::rand())),
    server(server),
    game_blocks_manager(server.getBlocksManager()),
    game_grounds_manager(server.getGroundsManager()),
    world_saver(saver)
{
}

World::World(Server& server, WorldSaver& saver, int seed) :
    entities(std::make_unique<EntitiesManager>(server)),
    generator(new NaturalGenerator(seed)),
    server(server),
    game_blocks_manager(server.getBlocksManager()),
    game_grounds_manager(server.getGroundsManager()),
    world_saver(saver)
{
}

World::~World()
{

}

void World::init()
{
    generator->init(game_blocks_manager, game_grounds_manager);
}

void World::sendToSubscribers(ECCPacket& packet, sf::Vector2i chunk) const
{
    ClientsManager& cm = server.getClientsManager();

    for (auto i = cm.getClientsBegin(); i != cm.getClientsEnd(); i++)
    {
        if (i->second->hasPlayer())
        {
            Player* player = i->second->getPlayer();

            bool subscribed = isChunkLoaded(chunk) && player->isSubscribedTo(chunk);

            if (subscribed)
                i->second->send(packet);
        }
    }
}

void World::sendToSubscribersWithException(ECCPacket& packet, sf::Vector2i chunk_a, sf::Vector2i chunk_b) const
{
    ClientsManager& cm = server.getClientsManager();

    for (auto i = cm.getClientsBegin(); i != cm.getClientsEnd(); i++)
    {
        if (i->second->hasPlayer())
        {
            Player* player = i->second->getPlayer();

            bool subscribed_a = isChunkLoaded(chunk_a) && player->isSubscribedTo(chunk_a);
            bool subscribed_b = isChunkLoaded(chunk_b) && player->isSubscribedTo(chunk_b);

            if (subscribed_a && !subscribed_b)
                i->second->send(packet);
        }
    }
}

const Chunk& World::getChunkConst(sf::Vector2i pos) const
{
    uint64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
        throw new std::out_of_range("World::getChunkConst : tried to access chunk that doesn't exist.");

    return *chunk_ptr->second;
}

Chunk& World::getChunk(sf::Vector2i pos)
{
    uint64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
    {
        log(INFO, "New chunk generated : {},{}\n", pos.x, pos.y);
        Chunk* new_chunk = new Chunk(*this, pos);
        chunks.emplace(key, std::unique_ptr<Chunk>(new_chunk));
        return *new_chunk;
    }

    return *chunk_ptr->second;
}

uint16_t World::getBlockId(sf::Vector2i pos)
{
    sf::Vector2i chunk_pos = getChunkPosFromBlockPos(pos);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
        return GameBlocks::ERROR->getId();
    return getChunkConst(chunk_pos).getBlockId(bp.x, bp.y);
}

uint16_t World::getGroundId(sf::Vector2i pos)
{
    sf::Vector2i chunk_pos = getChunkPosFromBlockPos(pos);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
        return 0;
    return getChunkConst(chunk_pos).getGroundId(bp.x, bp.y);
}

const Block* World::getBlockPtr(sf::Vector2i pos)
{
    return game_blocks_manager.getBlockByID(getBlockId(pos));
}

const Ground* World::getGroundPtr(sf::Vector2i pos)
{
    return game_grounds_manager.getGroundByID(getGroundId(pos));
}

void World::setBlock(sf::Vector2i pos, Block const * block)
{
    sf::Vector2i chunk = getChunkPosFromBlockPos(pos);

    if (!isChunkLoaded(chunk))
        return;

    getChunk(chunk).setBlock(getBlockPosInChunk(pos), block);

    SetTilePacket block_set(false, block->getId(), pos);
    sendToSubscribers(block_set, chunk);
}

void World::setGround(sf::Vector2i pos, Ground const * ground)
{
    sf::Vector2i chunk = getChunkPosFromBlockPos(pos);

    if (!isChunkLoaded(chunk))
        return;

    getChunk(chunk).setGround(getBlockPosInChunk(pos), ground);

    SetTilePacket ground_set(true, ground->getId(), pos);
    sendToSubscribers(ground_set, chunk);
}

sf::Vector2i World::getChunkPosFromBlockPos(sf::Vector2i block_pos)
{
    if (block_pos.x < -1)
        block_pos.x++;
    if (block_pos.y < -1)
        block_pos.y++;

    sf::Vector2i result(block_pos.x / Chunk::CHUNK_SIZE, block_pos.y / Chunk::CHUNK_SIZE);

    if (block_pos.x < 0)
        result.x--;
    if (block_pos.y < 0)
        result.y--;

    return result;
}

sf::Vector2i World::getBlockPosInChunk(sf::Vector2i block_pos)
{
    //sf::Vector2i chunk_pos = getChunkPosFromBlockPos(block_pos);
    sf::Vector2i result(block_pos.x % Chunk::CHUNK_SIZE, block_pos.y % Chunk::CHUNK_SIZE);

    if (result.x < 0)
        result.x += Chunk::CHUNK_SIZE;
    if (result.y < 0)
        result.y += Chunk::CHUNK_SIZE;

    assert(result.x >= 0);
    assert(result.y >= 0);
    assert(result.x < Chunk::CHUNK_SIZE);
    assert(result.y < Chunk::CHUNK_SIZE);

    return result;
}

void World::updateTileEntities(float delta_time)
{
    for (auto& c : chunks)
        c.second->updateTileEntities(delta_time);
}

void World::unloadOldChunks()
{
    if (last_unload_iteration.getElapsedTime().asSeconds() >= 3.f)
    {
        last_unload_iteration.restart();

        log(WARN, "Doing chunk unload iteration.\n");

        for (auto i = chunks.begin(); i != chunks.end(); )
        {
            if (!i->second->isOld())
            {
                i++;
            }
            else
            {
                bool cancel = false;
                for (auto j = server.getClientsManager().getClientsBegin(); j != server.getClientsManager().getClientsEnd(); j++)
                {
                    if (j->second->hasPlayer())
                        if (j->second->getPlayer()->isSubscribedTo(i->second->getPos()))
                        {
                            cancel = true;
                            break;
                        }
                }
                if (cancel)
                {
                    i->second->getPacket();
                    i++;
                    continue;
                }

                log(WARN, "Chunk {}; {} is being unloaded! Unloading chunks is not fully implemented yet, be careful.\n", i->second->getPos().x, i->second->getPos().y);

                WorldSaver::ChunkWithEntities cwe;

                auto popped_entities = entities->popEntitiesOfChunk(i->second->getPos());

                cwe.first.first = i->second->getPos();
                cwe.first.second = nullptr;
                cwe.second = popped_entities;

                if (i->second->hasBeenModified())
                {
                    cwe.first.second = i->second.release();
                }

                world_saver.addChunkToSave(cwe);

                i = chunks.erase(i);
            }
        }
    }
}

void World::saveAll()
{
    log(INFO, "Saving all chunks...");
    for (auto i = chunks.begin(); i != chunks.end(); )
    {
        WorldSaver::ChunkWithEntities cwe;

        auto popped_entities = entities->popEntitiesOfChunk(i->second->getPos());

        cwe.first.first = i->second->getPos();
        cwe.first.second = nullptr;
        cwe.second = popped_entities;

        if (i->second->hasBeenModified())
        {
            cwe.first.second = i->second.release();
        }

        world_saver.addChunkToSave(cwe);

        i = chunks.erase(i);
    }
    log(INFO, "Chunks saved!");
}
