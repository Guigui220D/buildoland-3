#include "Chunk.h"

#include "../Server.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "World.h"

#include <assert.h>
//TMP
#include <cstdlib>

#include "../../common/Networking/ServerToClientCodes.h"

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World* world, sf::Vector2i pos) :
    ready(false),
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 1),
    pos(pos),
    server(world->getServer()),
    world(world)
{
    //setBlock(0, 15, GameBlocks::GOLD);

    for (int x = 0; x < CHUNK_SIZE; x++)
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        setBlock(x, y, !(std::rand() % 5) ? (std::rand() % 5 + 2) : 0);
        setGround(x, y, std::rand() % 4 + 1);
    }
}

Chunk::~Chunk()
{
    //dtor
}

void Chunk::getPacket(sf::Packet& packet) const
{
    packet.clear();
    packet << (unsigned short)Networking::StoC::SendChunk;
    packet << getPos().x << getPos().y;

    for (int x = 0; x < CHUNK_SIZE; x++)
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        packet << getBlockId(x, y);
    }
    for (int x = 0; x < CHUNK_SIZE; x++)
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        packet << getGroundId(x, y);
    }
}

const Block* Chunk::getBlock(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return server->getBlocksManager().getBlockByID(getBlockId(x, y));
}

const Ground* Chunk::getGround(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return server->getGroundsManager().getGroundByID(getGroundId(x, y));
}

void Chunk::setBlock(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    blocks.set(x, y, id);
}

void Chunk::setGround(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    grounds.set(x, y, id);
}

void Chunk::setBlock(int x, int y, const Block* block)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    setBlock(x, y, block->getId());
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    setGround(x, y, ground->getId());
}
