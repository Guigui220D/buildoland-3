#include "Ground.h"

#include "../World/World.h"

const TilesetHelper<16, 16, 0> Ground::tilesetHelper;
const TilesetHelper<16, 8, 0> Ground::tilesetHelperDetails;

Ground::Ground(const std::string name) :
    name(name)
{
    //ctor
}

Ground::~Ground()
{
    //dtor
}

int Ground::getRandomInt(GroundInfo info) const
{
    //Pseudo random from seed and ground position
    return info.getWorld()->getSeed() * (info.getPos().x ^ 0xBCA0F521) + info.getWorld()->getSeed() + (info.getPos().y ^ 0x66ABC3F7);
}
