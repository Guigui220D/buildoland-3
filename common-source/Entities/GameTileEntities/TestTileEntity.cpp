#include "TestTileEntity.h"

#include <iostream>

TestTileEntity::TestTileEntity(World& world, unsigned int id, Chunk& chunk, sf::Vector2i tile_pos) :
    TileEntity(world, id, chunk, tile_pos)
{
    #ifdef CLIENT_SIDE
    rectangle.setPosition(getPosition());
    rectangle.setSize(sf::Vector2f(1.f, 1.f));
    rectangle.setFillColor(sf::Color::Yellow);
    #endif // CLIENT_SIDE
}

TestTileEntity::~TestTileEntity()
{
    //dtor
}

#ifdef CLIENT_SIDE
void TestTileEntity::draw(sf::RenderTarget& target) const
{
    std::cout << "DIIIII" << std::endl;
    target.draw(rectangle);
}
#endif
