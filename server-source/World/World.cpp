#include "World.h"

#include "../Utils/Utils.h"
#include "../Server/Server.h"

#include <iostream>

World::World(Server* server) :
    entities(server),
    server(server),
    game_blocks_manager(server->getBlocksManager()),
    game_grounds_manager(server->getGroundsManager())
{
    std::srand(time(0));
    seed = std::rand() << 16 | std::rand();
}

World::World(Server* server, int seed) :
    entities(server),
    server(server),
    game_blocks_manager(server->getBlocksManager()),
    game_grounds_manager(server->getGroundsManager()),
    seed(seed)
{
}

World::~World()
{

}

void World::sendToSubscribers(sf::Packet& packet, sf::Vector2i chunk) const
{
    ClientsManager& cm = server->getClientsManager();

    for (auto i = cm.getClientsBegin(); i != cm.getClientsEnd(); i++)
    {
        if (i->second->hasPlayer())
        {
            Player* player = i->second->getPlayer();

            bool subscribed = isChunkLoaded(chunk) && player->isSubscribedTo(&getChunkConst(chunk));

            if (subscribed)
                i->second->send(packet);
        }
    }
}

void World::sendToSubscribersWithException(sf::Packet& packet, sf::Vector2i chunk_a, sf::Vector2i chunk_b) const
{
    ClientsManager& cm = server->getClientsManager();

    for (auto i = cm.getClientsBegin(); i != cm.getClientsEnd(); i++)
    {
        if (i->second->hasPlayer())
        {
            Player* player = i->second->getPlayer();

            bool subscribed_a = isChunkLoaded(chunk_a) && player->isSubscribedTo(&getChunkConst(chunk_a));
            bool subscribed_b = isChunkLoaded(chunk_b) && player->isSubscribedTo(&getChunkConst(chunk_b));

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
        std::cout << "New chunk generated : " << pos.x << "; " << pos.y << std::endl;
        Chunk* new_chunk = new Chunk(this, pos);
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

const Block* World::getBlock(sf::Vector2i pos)
{
    return game_blocks_manager.getBlockByID(getBlockId(pos));
}

const Ground* World::getGround(sf::Vector2i pos)
{
    return game_grounds_manager.getGroundByID(getGroundId(pos));
}
