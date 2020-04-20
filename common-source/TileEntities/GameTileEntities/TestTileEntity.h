#pragma once

#include "../TileEntity.h"

#include <SFML/System/Clock.hpp>

class TestTileEntity : public TileEntity
{
    public:
        inline unsigned short getTypeCode() const override { return TileEntities::TestTE; };

        TestTileEntity(Chunk& chunk, sf::Vector2i pos);
        ~TestTileEntity();

        void update(float delta) override;

    private:
        sf::Clock clk;

};
