#pragma once

#include "../Ground.h"

class GroundError : public Ground
{
    public:
        GroundError();
        ~GroundError();

        bool acceptsTextureBleedings(GroundInfo info, const Ground* other) const;
        inline uint8_t getTextureRotation(GroundInfo info) const override { return 0; }

    protected:

    private:
};

