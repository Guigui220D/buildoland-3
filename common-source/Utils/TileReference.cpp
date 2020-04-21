#include "TileReference.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
    #include <exception>
#else
    #include "../../server-source/World/World.h"
#endif // CLIENT_SIDE

TileReference::TileReference(sf::Vector2i position, World& world) :
    pos(position),
    world(world),
    known_block(nullptr),
    known_ground(nullptr)
{}

TileReference::~TileReference()
{}

const Block* TileReference::getBlock() const
{
    if (!known_block)
        known_block = world.getBlockPtr(pos);

    return known_block;
}
void TileReference::setBlock(const Block* block)
{
    #ifndef CLIENT_SIDE
    world.setBlock(pos, block);
    known_block = block;
    #else
    throw new std::logic_error("Local world copy shall not be modified, do so through requests to the server.");
    #endif
}

const Ground* TileReference::getGround() const
{
    if (!known_ground)
        known_ground = world.getGroundPtr(pos);
    return known_ground;
}
void TileReference::setGround(const Ground* ground)
{
    #ifndef CLIENT_SIDE
    world.setGround(pos, ground);
    known_ground = ground;
    #else
    throw new std::logic_error("Local world copy shall not be modified, do so through requests to the server.");
    #endif
}

TileEntity* TileReference::getTileEntity() const
{
    #ifndef CLIENT_SIDE
    throw new std::logic_error("Server has no getTileEntity implemented for TileReference (i was lazy)");
    #else
    sf::Vector2i chunk = World::getChunkPosFromBlockPos(pos);

    if (!world.isChunkLoaded(chunk))
        return nullptr;

    return world.getChunk(chunk).getTileEntity(World::getBlockPosInChunk(pos));
    #endif // CLIENT_SIDE
}

const GameBlocks& TileReference::getBlocksManager() const
{
    return world.getBlocksManager();
}

const GameGrounds& TileReference::getGroundsManager() const
{
    return world.getGroundsManager();
}
