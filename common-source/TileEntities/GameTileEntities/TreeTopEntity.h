#pragma once

#include "../TileEntity.h"

#ifdef CLIENT_SIDE
    #include <SFML/Graphics/RectangleShape.hpp>
#endif // CLIENT_SIDE

class TreeTopEntity : public TileEntity
{
    public:
        inline unsigned short getTypeCode() const override { return TileEntities::TreeTopEntity; };

        TreeTopEntity(Chunk& chunk, sf::Vector2i pos);
        ~TreeTopEntity();

        #ifdef CLIENT_SIDE
        void drawAbove(sf::RenderTarget& target) const override;
        #endif

    private:
        #ifdef CLIENT_SIDE
        sf::RectangleShape rectangle;
        #endif // CLIENT_SIDE
};
