#pragma once

#include "../TileEntity.h"

#ifdef CLIENT_SIDE
    #include <SFML/Graphics.hpp>
#endif // CLIENT_SIDE

class TestTileEntity : public TileEntity
{
    public:
        inline unsigned short getTileEntityCode() const override { return TileEntities::TestTileEntity; };

        TestTileEntity(World& world, unsigned int id, sf::Vector2i tile_pos);
        ~TestTileEntity();

        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const override;
        #endif

    private:
        #ifdef CLIENT_SIDE
        sf::RectangleShape rectangle;
        #endif // CLIENT_SIDE
};
