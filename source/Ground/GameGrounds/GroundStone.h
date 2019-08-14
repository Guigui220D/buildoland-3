#pragma once

#include "../Ground.h"

class GroundStone : public Ground
{
    public:
        GroundStone();
        ~GroundStone();

        inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const override { return 1; }
        inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const override { return (ground_pos.x + ground_pos.y) % 4; }

    protected:

    private:
};

