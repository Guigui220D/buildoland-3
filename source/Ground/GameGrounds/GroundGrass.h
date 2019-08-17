#pragma once

#include "../Ground.h"

#include <iostream>

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info, int frame) const override { return 4; }

        virtual sf::VertexArray getSurfaceDetails(GroundInfo info, int frame) const override;

    protected:

    private:
};

