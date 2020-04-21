#include "Chunk.h"

#include "../Server/Server.h"

#include "../../common-source/Grounds/Ground.h"
#include "../../common-source/Blocks/Block.h"
#include "World.h"
#include "Generator.h"
#include "EntitiesManager.h"

#include "../../common-source/TileEntities/TileEntity.h"
#include "../../common-source/TileEntities/GameTileEntities/TestTileEntity.h"

#include "../../common-source/Networking/NetworkingCodes.h"

#include <exception>
#include <algorithm>

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

    if (tile_entities[y * CHUNK_SIZE + x])
    {
        tile_entities[y * CHUNK_SIZE + x].reset();
        cleanupTEList();
    }

    if (block->serverSideHasTE())
    {
        switch (block->getTileEntityCode())
        {
        case TileEntities::TestTE:
            tile_entities[y * CHUNK_SIZE + x].reset(new TestTileEntity(*this, getBlockPosInWorld(x, y)));
            actual_tile_entities.push_back(tile_entities[y * CHUNK_SIZE + x]);
            break;
        default:
        case TileEntities::None:
            throw std::logic_error("Block " + block->getName() + " has TE client-side but TE code says none or is unknown.");
        }
    }
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

//Just trying things here
void Chunk::cleanupTEList()
{
    #define v actual_tile_entities
    v.erase(std::remove_if(v.begin(), v.end(),
        [](std::shared_ptr<TileEntity>& te)
            { return (bool)(te.use_count() <= 1); }
        ), v.end());
    #undef v
}

void Chunk::updateTileEntities(float delta_time)
{
    #define v actual_tile_entities
    std::for_each(v.begin(), v.end(),
        [delta_time](std::shared_ptr<TileEntity>& te)
            { te->update(delta_time); });
    #undef v
}
