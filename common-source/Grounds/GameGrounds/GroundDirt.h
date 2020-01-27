#pragma once

#include "../Ground.h"

class GroundDirt : public Ground
{
    public:
        GroundDirt();
        ~GroundDirt();

        inline bool isShovelable(GroundInfo info) const override { return false; }

    protected:

    private:
};

