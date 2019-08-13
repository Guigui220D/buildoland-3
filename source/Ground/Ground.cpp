#include "Ground.h"

Ground::Ground(const std::string name) :
    name(name)
{
    //ctor
}

Ground::~Ground()
{
    //dtor
}

uint32_t Ground::getTextureNumber(sf::Vector2i ground_pos) const
{
    return 0;
}

uint8_t Ground::getTextureRotation(sf::Vector2i ground_pos) const
{
    return 0;
}
