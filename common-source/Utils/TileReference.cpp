#include "TileReference.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
    #include <exception>
#else
    #include "../../server-source/World/World.h"
#endif // CLIENT_SIDE

TileReference::TileReference(sf::Vector2i position, World& world) :
    pos(position),
    world(world)
{}

TileReference::~TileReference()
{}

TileReference::operator const Block*() const
{
    return world.getBlockPtr(pos);
}
void TileReference::operator=(const Block* block)
{
    #ifndef CLIENT_SIDE
    world.setBlock(pos, block);
    #else
    throw new std::logic_error("Local world copy shall not be modified, do so through requests to the server.");
    #endif
}

const Block* TileReference::getBlock() const
{
    return world.getBlockPtr(pos);
}
void TileReference::setBlock(const Block* block)
{
    #ifndef CLIENT_SIDE
    world.setBlock(pos, block);
    #else
    throw new std::logic_error("Local world copy shall not be modified, do so through requests to the server.");
    #endif
}

const Ground* TileReference::getGround() const
{
    return world.getGroundPtr(pos);
}
void TileReference::setGround(const Ground* ground)
{
    #ifndef CLIENT_SIDE
    world.setGround(pos, ground);
    #else
    throw new std::logic_error("Local world copy shall not be modified, do so through requests to the server.");
    #endif
}
