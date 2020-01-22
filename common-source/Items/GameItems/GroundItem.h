#pragma once

#include "../Item.h"

#include "../../Grounds/Ground.h"

class GroundItem : public Item
{
    public:
        GroundItem(Ground const * ground);
        virtual ~GroundItem();

        inline Ground const * getGround() { return ground; }

    protected:
        Ground const * const ground;

    private:
};
