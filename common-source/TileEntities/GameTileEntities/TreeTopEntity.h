#pragma once

/*

#include "../TileEntity.h"

#ifdef CLIENT_SIDE
    #include <SFML/Graphics/RectangleShape.hpp>
#endif // CLIENT_SIDE

class TreeTopEntity : public TileEntity
{
    public:
        inline unsigned short getTileEntityCode() const override { return TileEntities::TreeTopEntity; };

        TreeTopEntity(World& world, unsigned int id, sf::Vector2i tile_pos);
        ~TreeTopEntity();

        #ifdef CLIENT_SIDE
        void drawAbove(sf::RenderTarget& target) const override;
        #endif

    private:
        #ifdef CLIENT_SIDE
        sf::RectangleShape rectangle;
        #endif // CLIENT_SIDE
};

*/
