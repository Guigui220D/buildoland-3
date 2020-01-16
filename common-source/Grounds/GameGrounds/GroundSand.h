#pragma once

#include "../Ground.h"

class GroundSand : public Ground
{
    public:
        GroundSand();
        ~GroundSand();

        #ifdef CLIENT_SIDE
        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info, int frame) const override { return 3; }
        #endif
    protected:

    private:
};

