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
            top.verts[i].position += sf::Vector2f(0, -0.5f);
    return top;
}

TextQuad Block::getSideVertices(BlockInfo info) const
{
    TextQuad side = utils::getSquare(Block::tilesetHelper.getFourVertices(getSideTextureNumber(info), 0), info.getPos());
    for (int i = 0; i < 2; i++)
        side.verts[i].position += sf::Vector2f(0, 0.5f);
    if (darkenSide())
        for (int i = 0; i < 4; i++)
            side.verts[i].color = sf::Color(127, 127, 127);
    return side;
}
