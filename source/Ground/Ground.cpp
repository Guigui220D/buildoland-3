#include "Ground.h"

#include "../World/World.h"

#include "../Utils/XXHash.hpp"
#include "../Utils/XXHash_bis.hpp"

const TilesetHelper<16, 16, 0> Ground::tilesetHelper;
const TilesetHelper<16, 8, 0> Ground::tilesetHelperDetails;

Ground::Ground(const std::string name, uint32_t default_texture) :
    name(name),
    default_texture(default_texture)
{
    //ctor
}

Ground::~Ground()
{
    //dtor
}

uint32_t Ground::getRandomInt(GroundInfo info)
{
    //Pseudo random from seed and ground position
    sf::Vector2i pos = info.getPos();
    return XXH32(&pos, sizeof(pos), info.getWorld()->getSeed());
}
