#include "GroundBlock.h"

GroundBlock::GroundBlock(const std::string name, bool should_have_item, uint32_t default_textures_begin, uint32_t default_textures_count) :
    Block(name, should_have_item, default_textures_begin),
    default_textures_count(default_textures_count)
{
    //ctor
}

GroundBlock::~GroundBlock()
{
    //dtor
}

#ifdef CLIENT_SIDE
TextQuad GroundBlock::getTopVertices(TileReference info) const
{
    TextQuad top = utils::getSquare(Block::tilesetHelper.getFourVertices(getTopTextureNumber(info), getRandomInt(info, 5678)), info.getPos());
    return top;
}
#endif
