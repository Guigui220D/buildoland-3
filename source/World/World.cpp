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

}

const Chunk& World::getChunkConst(sf::Vector2i pos) const
{
    int64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
        throw new std::out_of_range("World::getChunkConst : tried to access chunk that doesn't exist.");

    return *chunk_ptr->second;
}

Chunk& World::getChunk(sf::Vector2i pos)
{
    int64_t key = utils::combine(pos.x, pos.y);
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

uint16_t World::getBlockId(sf::Vector2i pos, bool load)
{
    sf::Vector2i chunk_pos = sf::Vector2i(pos.x / Chunk::CHUNK_SIZE, pos.y / Chunk::CHUNK_SIZE);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
    {
        if (load)
        {
            int64_t key = utils::combine(chunk_pos.x, chunk_pos.y);
            std::cout << "New chunk generated : " << chunk_pos.x << "; " << chunk_pos.y << std::endl;
            Chunk* new_chunk = new Chunk(this, chunk_pos);
            chunks.emplace(key, std::unique_ptr<Chunk>(new_chunk));
            return new_chunk->getBlockId(bp.x, bp.y);
        }
        else
            return 0;
    }
    return getChunk(chunk_pos).getBlockId(bp.x, bp.y);
}

uint16_t World::getGroundId(sf::Vector2i pos, bool load)
{
    sf::Vector2i chunk_pos = sf::Vector2i(pos.x / Chunk::CHUNK_SIZE, pos.y / Chunk::CHUNK_SIZE);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
    {
        if (load)
        {
            int64_t key = utils::combine(chunk_pos.x, chunk_pos.y);
            std::cout << "New chunk generated : " << chunk_pos.x << "; " << chunk_pos.y << std::endl;
            Chunk* new_chunk = new Chunk(this, chunk_pos);
            chunks.emplace(key, std::unique_ptr<Chunk>(new_chunk));
            return new_chunk->getGroundId(bp.x, bp.y);
        }
        else
            return 0;
    }
    return getChunk(chunk_pos).getGroundId(bp.x, bp.y);
}

const Block* World::getBlock(sf::Vector2i pos, bool load)
{
    return gameBlocksManager.getBlockByID(getBlockId(pos, load));
}

const Ground* World::getGround(sf::Vector2i pos, bool load)
{
    return gameGroundsManager.getGroundByID(getGroundId(pos, load));
}
