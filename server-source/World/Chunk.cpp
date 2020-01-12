#include "Chunk.h"

#include "../Server/Server.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "World.h"

#include <assert.h>
//TMP
#include <cstdlib>

#include "../../common-source/Networking/NetworkingCodes.h"

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World* world, sf::Vector2i pos) :
    ready(false),
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 1),
    pos(pos),
    server(world->getServer()),
    world(world),
    packet(new sf::Packet()),
    packet_ready(false)
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        if (x % 2 && y % 2)
            setBlock(x, y, std::rand() % 4 + 2);
        setGround(x, y, GameGrounds::GRASS);
    }

    setBlock(0, CHUNK_SIZE - 1, GameBlocks::IRON);
    setBlock(CHUNK_SIZE - 1, 0, GameBlocks::IRON);
    setBlock(CHUNK_SIZE - 1, CHUNK_SIZE - 1, GameBlocks::GOLD);
}

Chunk::~Chunk()
{
    //dtor
}

void Chunk::generatePacket()
{
    packet->clear();
    (*packet) << (unsigned short)Networking::StoC::SendChunk;
    (*packet) << getPos().x << getPos().y;

    packet->append(blocks.getData(), blocks.getDataSize());
    packet->append(grounds.getData(), grounds.getDataSize());

    packet_ready = true;
}

sf::Packet& Chunk::getPacket()
{
    if (!packet_ready)
        generatePacket();

    return (*packet);
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
    packet_ready = false;

    sf::Packet block_set;
    block_set << (unsigned short)Networking::StoC::BlockUpdate;
    block_set << x << y;
    block_set << id;

    world->sendToSubscribers(block_set, pos);
}

void Chunk::setGround(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    grounds.set(x, y, id);
    packet_ready = false;

    sf::Packet ground_set;
    ground_set << (unsigned short)Networking::StoC::GroundUpdate;
    ground_set << x << y;
    ground_set << id;

    world->sendToSubscribers(ground_set, pos);
}

void Chunk::setBlock(int x, int y, const Block* block)
{
    setBlock(x, y, block->getId());
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    setGround(x, y, ground->getId());
}
