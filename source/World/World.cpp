#include "World.h"

#include "../Utils/Utils.h"

#include <iostream>

World::World(Game* game) :
    game(game)
{
    std::srand(time(0));
    seed = std::rand() << 16 | std::rand();
}

World::World(Game* game, int seed) :
    game(game),
    seed(seed)
{
}

World::~World()
{
    //dtor
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
