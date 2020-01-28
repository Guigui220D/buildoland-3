#include "BlockBoulder.h"

BlockBoulder::BlockBoulder(const std::string name, uint32_t top_texture, uint32_t side_texture) :
    Block(name, false, top_texture),
    side_texture(side_texture)
{
    //ctor
}

BlockBoulder::~BlockBoulder()
{
    //dtor
}
