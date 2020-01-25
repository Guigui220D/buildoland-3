#include "StaticGround.h"

StaticGround::StaticGround(const std::string name, bool should_have_item, uint32_t default_texture) :
    Ground(name, should_have_item, default_texture)
{
    //ctor
}

StaticGround::~StaticGround()
{
    //dtor
}
