#pragma once

#include "../Ground.h"

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        inline uint32_t getTextureNumber(GroundInfo info) const override { return 3; }
        inline uint8_t getTextureRotation(GroundInfo info) const override { return (info.getPos().x + info.getPos().y) % 4; }

        inline bool hasSurfaceDetails(GroundInfo info) const override { return true; }
        inline uint32_t getSurfaceDetailNumber(GroundInfo info) const override
        {
            if (std::rand() % 10 == 0)
                return 8;
            if (std::rand() % 10 == 0)
                return 9;
            return 0;
        }
        inline uint8_t getSurfaceDetailRotation(GroundInfo info) const
        {
            return std::rand() % 4;
        }

    protected:

    private:
};

