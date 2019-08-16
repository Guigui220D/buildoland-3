#pragma once

#include "../Ground.h"

class GroundWater : public Ground
{
    public:
        GroundWater();
        ~GroundWater();

        inline uint8_t getTextureRotation(GroundInfo info) const override { return 0; }

        virtual sf::VertexArray getSurfaceDetails(GroundInfo info, int frame) const override;

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info) const override { return 5; }

    protected:

    private:
};

