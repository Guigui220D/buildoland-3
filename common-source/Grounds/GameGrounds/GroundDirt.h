#pragma once

#include "../Ground.h"

class GroundDirt : public Ground
{
    public:
        GroundDirt();
        ~GroundDirt();

        inline bool isShovelable(TileReference info) const override { return false; }

        #ifdef CLIENT_SIDE
        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 5; }
        #endif

    protected:

    private:
};

