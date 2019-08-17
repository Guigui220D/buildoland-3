#include "Block.h"

const TilesetHelper<16, 16, 0> Block::tilesetHelper;

Block::Block(const std::string name,uint32_t default_texture) :
    name(name),
    default_texture(default_texture)
{
    //ctor
}

Block::~Block()
{
    //dtor
}

TextQuad Block::getTopVertices(BlockInfo info) const
{
    TextQuad top = utils::getSquare(Block::tilesetHelper.getFourVertices(getTopTextureNumber(info), 0), info.getPos());
    if (hasVolume(info))
        for (int i = 0; i < 4; i++)
            top.verts[i].position += sf::Vector2f(0, -0.3f);
    return top;
}
