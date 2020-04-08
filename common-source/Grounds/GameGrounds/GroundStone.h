#pragma once

#include "../Ground.h"

class GroundStone : public Ground
{
    public:
        GroundStone();
        ~GroundStone();

        #ifdef CLIENT_SIDE
        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 2; }
        #endif
    protected:

    private:
};

