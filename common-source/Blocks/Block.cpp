#include "Block.h"

#ifdef CLIENT_SIDE
const TilesetHelper<16, 16, 1> Block::tilesetHelper;

Block::Block(const std::string name, bool should_have_item, uint32_t default_texture) :
    has_item(should_have_item),
    name(name),
    default_texture(default_texture)
{
    //ctor
}
#else
Block::Block(const std::string name, bool should_have_item, uint32_t default_texture) :
    has_item(should_have_item),
    name(name)
{
    //ctor
}
#endif

Block::~Block()
{
    //dtor
}

#ifdef CLIENT_SIDE
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
#endif
