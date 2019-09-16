#include "World.h"

#include "../Utils/Utils.h"
#include "../Game.h"

#include <iostream>

int unsigned World::RENDER_DISTANCE = 3;

World::World(Game* game) :
    game(game),
    gameBlocksManager(game->getBlocksManager()),
    gameGroundsManager(game->getGroundsManager())
{
    std::srand(time(0));
    seed = std::rand() << 16 | std::rand();
}

World::World(Game* game, int seed) :
    game(game),
    gameBlocksManager(game->getBlocksManager()),
    gameGroundsManager(game->getGroundsManager()),
    seed(seed)
{
}

World::~World()
{
    //dtor
}

void World::updateLoadedChunk(sf::Vector2f center)
{
    //Remove chuncks too far of center
    //Add chuncks not added, near center

    //Unload all the chunks that need to be removed
    for (auto i = chunks.begin(); i != chunks.end();)
    {
        if (i->second->to_be_removed)
        {
            //Removes the chunk from the map
            i = chunks.erase(i);
        }
        else
            i++;
    }
}

bool World::addChunk(sf::Packet packet)
{
    //We expect the packet to be of that size
    //4 bytes per tile (2 for block and 2 for ground)
    //8 bytes for the position
    //2 bytes for the packet header
    size_t expected_packet_size = Chunk::getChunkDataSize();
    expected_packet_size += sizeof(int) * 2; //Position
    expected_packet_size += 2;

    if (packet.getDataSize() < expected_packet_size)
    {
        std::cerr << "Chunk packet is too small! Expected " << expected_packet_size << " bytes, got " << packet.getDataSize() << " bytes." << std::endl;
        return false;
    }

    //Get chunk position
    sf::Vector2i pos;
    packet >> pos.x;
    packet >> pos.y;

    uint64_t key = utils::combine(pos.x, pos.y);

    //Remove chunk if one already exists with that position
    auto it = chunks.find(key);
    if (it != chunks.end())
        chunks.erase(it);

    //Construct new chunk
    bool success = true;

    Chunk* new_chunk = new Chunk(this, pos, packet, success);

    if (!success)
    {
        std::cerr << "A new chunk packet couldn't be parsed" << std::endl;
        delete new_chunk;
        return false;
    }

    //Add the new chunk!
    chunks.emplace(key, std::unique_ptr<Chunk>(new_chunk));
    return true;
}

const Chunk& World::getChunkConst(sf::Vector2i pos) const
{
    uint64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
        throw new std::out_of_range("World::getChunkConst : tried to access chunk that doesn't exist.");

    return *chunk_ptr->second;
}

/*
Chunk& World::getChunk(sf::Vector2i pos)
{
    throw std::logic_error("World::getChunk(sf::Vector2i pos) : Not implemented!");

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
*/

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
