#pragma once

#include <string>
#include <SFML/System.hpp>

#include "../Utils/TilesetHelper.h"

class Ground
{
    public:
        static const TilesetHelper<16, 16, 0> tilesetHelper;
        static const TilesetHelper<16, 8, 0> tilesetHelperDetails;

        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

        virtual inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const { return 0; }
        virtual inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const { return 0; }

        inline TextRect getTextureVertices(sf::Vector2i ground_pos) const { return tilesetHelper.getFourVertices(getTextureNumber(ground_pos), getTextureRotation(ground_pos)); }

        virtual inline bool hasSurfaceDetails(sf::Vector2i ground_pos) const { return false; }
        virtual inline uint32_t getSurfaceDetailNumber(sf::Vector2i ground_pos) const { return 0; }
        virtual inline uint8_t getSurfaceDetailRotation(sf::Vector2i ground_pos) const { return 0; }

        inline TextRect getSurfaceDetailVertices(sf::Vector2i ground_pos) const { return tilesetHelperDetails.getFourVertices(getSurfaceDetailNumber(ground_pos), getSurfaceDetailRotation(ground_pos)); }

    private:
        std::string name;
};
