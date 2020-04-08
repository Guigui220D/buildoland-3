#pragma once

#include "../Ground.h"

class GroundError : public Ground
{
    public:
        GroundError();
        ~GroundError();

        inline bool isShovelable(TileReference info) const override { return false; }

        #ifdef CLIENT_SIDE
        bool acceptsTextureBleedings(TileReference info, const Ground* other) const;
        inline uint8_t getTextureRotation(TileReference info) const override { return 0; }
        #endif
    protected:

    private:
};

