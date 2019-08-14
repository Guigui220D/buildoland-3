#pragma once

#include "../Ground.h"

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const override { return 3; }
        inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const override { return (ground_pos.x + ground_pos.y) % 4; }

        inline bool hasSurfaceDetails(sf::Vector2i ground_pos) const override { return true; }
        inline uint32_t getSurfaceDetailNumber(sf::Vector2i ground_pos) const override
        {
            if ((ground_pos.x + ground_pos.y) % 5 == 4)
            {
                return 8;
            }
            if ((ground_pos.x + ground_pos.y) % 7 == 6)
            {
                return 9;
            }
            return 0;
        }

    protected:

    private:
};

