#include "TestTileEntity.h"

#include "../../Utils/Log.h"

TestTileEntity::TestTileEntity(Chunk& chunk, sf::Vector2i pos) :
    TileEntity(chunk, pos)
{
    log(INFO, "Test_te : Hello! from {}; {}.\n", getTilePos().x, getTilePos().y);
}

TestTileEntity::~TestTileEntity()
{
    log(INFO, "Test_te : Goodbye. from {}; {}.\n", getTilePos().x, getTilePos().y);
}

void TestTileEntity::update(float delta)
{
    #ifndef CLIENT_SIDE
    if (clk.getElapsedTime().asSeconds() > 1.f)
    {
        clk.restart();
        log(INFO, "Test_te : I exist; from {}; {}.\n", getTilePos().x, getTilePos().y);
    }
    #endif
}
