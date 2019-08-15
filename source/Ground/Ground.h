#pragma once

#include <string>
#include <SFML/System.hpp>

#include "../Utils/TilesetHelper.h"
#include "../Utils/Utils.h"

#include "GroundInfo.h"

class Ground
{
    friend class GameGrounds;
    public:
        static const TilesetHelper<16, 16, 0> tilesetHelper;
        static const TilesetHelper<16, 8, 0> tilesetHelperDetails;

        Ground(const std::string name, uint32_t default_texture);
        virtual ~Ground();

        inline const std::string getName() const { return name; }
        inline uint16_t getId() const { return id; }

        inline Quad getTextureVertices(GroundInfo info) const { return tilesetHelper.getFourVertices(getTextureNumber(info), getTextureRotation(info)); }

        virtual inline bool hasSurfaceDetails(GroundInfo info) const { return true; }
        virtual sf::VertexArray getSurfaceDetails(GroundInfo info) const;

    protected:
        virtual inline uint32_t getTextureNumber(GroundInfo info) const { return default_texture; }
        virtual inline uint8_t getTextureRotation(GroundInfo info) const { return getRandomInt(info) % 4; }

        virtual inline bool hasTextureBleedings() const { return false; }
        virtual inline uint32_t getBleedingForNeighborGrounds(GroundInfo info) const { return 0; }

        virtual bool acceptsTextureBleedings(GroundInfo info, const Ground* other) const;

        void addNeighborsBleeding(GroundInfo info, sf::VertexArray& vertex_array) const;

        static uint32_t getRandomInt(GroundInfo info);

    private:
        std::string name;
        mutable uint16_t id;

        uint32_t default_texture;
};
