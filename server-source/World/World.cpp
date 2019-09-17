#include "World.h"

#include "../Utils/Utils.h"
#include "../Server.h"

#include <iostream>

World::World(Server* server) :
    server(server),
    gameBlocksManager(server->getBlocksManager()),
    gameGroundsManager(server->getGroundsManager())
{
    std::srand(time(0));
    seed = std::rand() << 16 | std::rand();
}

World::World(Server* server, int seed) :
    server(server),
    gameBlocksManager(server->getBlocksManager()),
    gameGroundsManager(server->getGroundsManager()),
    seed(seed)
{
}

World::~World()
{

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
        return 0;
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
    return gameBlocksManager.getBlockByID(getBlockId(pos));
}

const Ground* World::getGround(sf::Vector2i pos)
{
    return gameGroundsManager.getGroundByID(getGroundId(pos));
}
