#pragma once

#include "../Ground.h"

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const override { return 3; }
        inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const override { return (ground_pos.x + ground_pos.y) % 4; }

    protected:

    private:
};

