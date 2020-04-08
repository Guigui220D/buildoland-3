#include "TileReference.h"

#ifdef CLIENT_SIDE

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
    return world.getBlock(pos);
}
void TileReference::operator=(const Block* block)
{
    world.setBlock(pos, block);
}

const Block* TileReference::getBlock() const
{
    return world.getBlock(pos);
}
void TileReference::setBlock(const Block* block)
{
    world.setBlock(pos, block);
}

const Ground* TileReference::getGround() const
{
    return world.getGround(pos);
}
void TileReference::setGround(const Ground* ground)
{
    world.setGround(pos, ground);
}
