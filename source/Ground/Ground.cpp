#include "Ground.h"

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
