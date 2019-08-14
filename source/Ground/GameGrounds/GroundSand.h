#pragma once

#include "../Ground.h"

class GroundSand : public Ground
{
    public:
        GroundSand();
        ~GroundSand();

        inline uint32_t getTextureNumber(GroundInfo info) const override { return 2; }
        inline uint8_t getTextureRotation(GroundInfo info) const override { return (info.getPos().x + info.getPos().y) % 4; }

    protected:

    private:
};

