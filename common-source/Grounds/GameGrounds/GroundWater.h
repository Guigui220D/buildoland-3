#pragma once

#include "../Ground.h"

class GroundWater : public Ground
{
    public:
        GroundWater();
        ~GroundWater();

        inline bool isShovelable(TileReference info) const override { return false; }

        #ifdef CLIENT_SIDE
        virtual sf::VertexArray getSurfaceDetails(TileReference info, int frame) const override;

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 24 + frame; }
        #endif
    protected:

    private:
};

