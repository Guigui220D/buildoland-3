#pragma once

#include "../Ground.h"

class GroundStone : public Ground
{
    public:
        GroundStone();
        ~GroundStone();

        inline uint32_t getTextureNumber(GroundInfo info) const override { return 1; }
        inline uint8_t getTextureRotation(GroundInfo info) const override { return (info.getPos().x + info.getPos().y) % 4; }

    protected:

    private:
};

