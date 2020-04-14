#pragma once

#include "../Ground.h"

class GroundParquet : public Ground
{
    public:
        GroundParquet();
        ~GroundParquet();

        #ifdef CLIENT_SIDE
        bool acceptsTextureBleedings(TileReference info, const Ground* other) const override;
        inline uint8_t getTextureRotation(TileReference info) const override { return 0; }

        inline bool isNatural() const override { return false; }
        #endif
    protected:

    private:
};

