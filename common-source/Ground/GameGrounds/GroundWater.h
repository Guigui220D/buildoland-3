#pragma once

#include "../Ground.h"

class GroundWater : public Ground
{
    public:
        GroundWater();
        ~GroundWater();

        #ifdef CLIENT_SIDE
        virtual sf::VertexArray getSurfaceDetails(GroundInfo info, int frame) const override;

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info, int frame) const override { return 24 + frame; }
        #endif
    protected:

    private:
};

