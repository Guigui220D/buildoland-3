#include "BlockTree.h"

BlockTree::BlockTree() :
    Block("tree", 0)
{
    //ctor
}

BlockTree::~BlockTree()
{
    //dtor
}

/*
TextQuad BlockTree::getTopVertices(BlockInfo info) const
{
    TextQuad top = utils::getSquare(Block::tilesetHelper.getFourVertices(getTopTextureNumber(info), 0, 3, 3), info.getPos());
    top.verts[1].position += sf::Vector2f(2.f, 0);
    top.verts[2].position += sf::Vector2f(2.f, 2.f);
    top.verts[3].position += sf::Vector2f(0, 2.f);
    for (int i = 0; i < 4; i++)
        top.verts[i].position += sf::Vector2f(-1.f, -2.5f);
    return top;
}
*/

TextQuad BlockTree::getSideVertices(BlockInfo info) const
{
    TextQuad side = utils::getSquare(Block::tilesetHelper.getFourVertices(getSideTextureNumber(info), 0), info.getPos());
    for (int i = 0; i < 2; i++)
    {
        side.verts[i].texCoords += sf::Vector2f(0, -16.f);
        side.verts[i].position += sf::Vector2f(0, -1.f);
    }
    return side;
}

