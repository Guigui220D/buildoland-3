#pragma once

#include <string>
#include <SFML/System.hpp>

#include "../Utils/TilesetHelper.h"
#include "../Utils/Utils.h"

#include "GroundInfo.h"

class Ground
{
    public:
        static const TilesetHelper<16, 16, 0> tilesetHelper;
        static const TilesetHelper<16, 8, 0> tilesetHelperDetails;

        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

        virtual inline uint32_t getTextureNumber(GroundInfo info) const { return 0; }
        virtual inline uint8_t getTextureRotation(GroundInfo info) const { return 0; }

        inline TextRect getTextureVertices(GroundInfo info) const { return tilesetHelper.getFourVertices(getTextureNumber(info), getTextureRotation(info)); }

        virtual inline bool hasSurfaceDetails(GroundInfo info) const { return false; }
        virtual inline uint32_t getSurfaceDetailNumber(GroundInfo info) const { return 0; }
        virtual inline uint8_t getSurfaceDetailRotation(GroundInfo info) const { return 0; }

        inline TextRect getSurfaceDetailVertices(GroundInfo info) const { return tilesetHelperDetails.getFourVertices(getSurfaceDetailNumber(info), getSurfaceDetailRotation(info)); }

        int getRandomInt(GroundInfo info) const;

    private:
        std::string name;
};
