#pragma once

#include "../Item.h"

#include "../../Grounds/Ground.h"

class GroundItem : public Item
{
    public:
        GroundItem(Ground const * ground);
        virtual ~GroundItem();

        inline Ground const * getGround() { return ground; }

        #ifdef CLIENT_SIDE
        virtual inline TextureSet getTexturesSet() const { return TextureSet::GroundsTextureSet; }
        #endif // CLIENT_SIDE

    protected:
        Ground const * const ground;

    private:
};
