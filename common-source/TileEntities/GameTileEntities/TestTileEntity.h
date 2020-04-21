#pragma once

#include "../TileEntity.h"

#include <SFML/System/Clock.hpp>

#ifdef CLIENT_SIDE
    #include <SFML/Graphics/CircleShape.hpp>
#endif // CLIENT_SIDE

class TestTileEntity : public TileEntity
{
    public:
        inline unsigned short getTypeCode() const override { return TileEntities::TestTE; };

        TestTileEntity(Chunk& chunk, sf::Vector2i pos);
        ~TestTileEntity();

        void update(float delta) override;

        #ifdef CLIENT_SIDE
        virtual void draw(sf::RenderTarget& target) const;
        virtual bool readTileEntityPacket(ECCPacket& packet);
        #endif

    private:
        sf::Clock clk;

        #ifdef CLIENT_SIDE
        sf::CircleShape cs;
        #endif // CLIENT_SIDE
};
