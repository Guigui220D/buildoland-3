#pragma once

#include "../Ground.h"

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        #ifdef CLIENT_SIDE
        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 4; }

        virtual sf::VertexArray getSurfaceDetails(TileReference info, int frame) const override;
        #endif
    protected:

    private:
};

