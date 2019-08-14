#include "Ground.h"

const TilesetHelper<16, 16, 0> Ground::tilesetHelper;

Ground::Ground(const std::string name) :
    name(name)
{
    //ctor
}

Ground::~Ground()
{
    //dtor
}
