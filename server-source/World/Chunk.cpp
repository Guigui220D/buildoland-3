#include "Chunk.h"

#include "../Server/Server.h"

#include "../../common-source/Grounds/Ground.h"
#include "../../common-source/Blocks/Block.h"
#include "World.h"

#include <assert.h>
//TMP
#include <cstdlib>

//TEST
#include "../../common-source/Entities/GameTileEntities/TestTileEntity.h"

#include "../../common-source/Networking/NetworkingCodes.h"

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World& world, sf::Vector2i pos) :
    ready(false),
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 1),
    tile_entities(CHUNK_SIZE, CHUNK_SIZE, nullptr),
    pos(pos),
    server(world.getServer()),
    world(world),
    packet(new sf::Packet()),
    packet_ready(false)
{
    world.getGenerator()->generateChunk(this);

    TestTileEntity* te = new TestTileEntity(world, world.getEntityManager().getNextEntityId(), getBlockPosInWorld(0, 0));
    te->assignChunk(this);
    world.getEntityManager().newEntity(te, false);


    ready = true;
}

Chunk::~Chunk()
{
    for (int i = 0; i < tile_entities.getDataSize(); i++)
    {
        tile_entities.getData()[i]->assignChunk(nullptr);
        //tile_entities.getData()[i]->to_be_destroyed = true;
    }

    //dtor
}

void Chunk::generatePacket()
{
    packet->clear();
    (*packet) << Networking::StoC::SendChunk;
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
    return server.getBlocksManager().getBlockByID(getBlockId(x, y));
}

const Ground* Chunk::getGround(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return server.getGroundsManager().getGroundByID(getGroundId(x, y));
}

void Chunk::setBlock(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    blocks.set(x, y, id);
    packet_ready = false;
}

void Chunk::setGround(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    grounds.set(x, y, id);
    packet_ready = false;
}

void Chunk::setBlock(int x, int y, const Block* block)
{
    setBlock(x, y, block->getId());
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    setGround(x, y, ground->getId());
}
