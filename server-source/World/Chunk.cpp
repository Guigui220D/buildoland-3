#include "Chunk.h"

#include "../Server/Server.h"

#include "../../common-source/Grounds/Ground.h"
#include "../../common-source/Blocks/Block.h"
#include "World.h"
#include "Generator.h"
#include "EntitiesManager.h"

#include <cassert>

//TEST
#include "../../common-source/Entities/GameTileEntities/TestTileEntity.h"
#include "../../common-source/Entities/GameTileEntities/TreeTopEntity.h"

#include "../../common-source/Networking/NetworkingCodes.h"

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World& world, sf::Vector2i pos) :
    ready(false),
    blocks(CHUNK_SIZE*CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE*CHUNK_SIZE, 1),
    tile_entities(CHUNK_SIZE * CHUNK_SIZE, nullptr),
    pos(pos),
    server(world.getServer()),
    world(world),
    packet(new ECCPacket()),
    packet_ready(false)
{
    world.getGenerator()->generateChunk(this);

    ready = true;
}

Chunk::~Chunk()
{
}

void Chunk::generatePacket()
{
    packet->clear();
    (*packet) << Networking::StoC::SendChunk;
    (*packet) << getPos().x << getPos().y;

    packet->append(blocks.data(), blocks.size() * sizeof(blocks[0]));
    packet->append(grounds.data(), grounds.size() * sizeof(grounds[0]));

    packet_ready = true;
}

ECCPacket& Chunk::getPacket()
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

void Chunk::setBlock(int x, int y, const Block* block)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);

    blocks[y*CHUNK_SIZE + x] = block->getId();

    packet_ready = false;
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);

    grounds[y*CHUNK_SIZE + x] = ground->getId();

    packet_ready = false;
}
