#pragma once

#include <string>
#include <SFML/System.hpp>

#include "../Utils/TilesetHelper.h"

class Ground
{
    public:
        static const TilesetHelper<16, 16, 0> tilesetHelper;

        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

        virtual inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const { return 0; }
        virtual inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const { return 0; }
        inline sf::FloatRect getTextureFloatRect(sf::Vector2i ground_pos) const { return tilesetHelper.getFloatRect(getTextureNumber(ground_pos)); }
        inline TextRect getTextureVertices(sf::Vector2i ground_pos) const { return tilesetHelper.getFourVertices(getTextureNumber(ground_pos)); }

    private:
        std::string name;
};
