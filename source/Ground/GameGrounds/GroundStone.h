#pragma once

#include "../Ground.h"

class GroundStone : public Ground
{
    public:
        GroundStone();
        ~GroundStone();

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info) const override { return 2; }

    protected:

    private:
};

