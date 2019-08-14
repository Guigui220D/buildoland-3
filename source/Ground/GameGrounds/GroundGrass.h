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
            if (std::rand() % 10 == 0)
                return 8;
            if (std::rand() % 10 == 0)
                return 9;
            return 0;
        }
        inline uint8_t getSurfaceDetailRotation(sf::Vector2i ground_pos) const
        {
            return std::rand() % 4;
        }

    protected:

    private:
};

