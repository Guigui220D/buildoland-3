#pragma once

#include "../Ground.h"

class GroundDirt : public Ground
{
    public:
        GroundDirt();
        ~GroundDirt();

        inline bool isShovelable(TileReference info) const override { return false; }

    protected:

    private:
};

