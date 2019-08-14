#pragma once

#include "../Ground.h"

class GroundSand : public Ground
{
    public:
        GroundSand();
        ~GroundSand();

        inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const override { return 2; }
        inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const override { return (ground_pos.x + ground_pos.y) % 4; }

    protected:

    private:
};

