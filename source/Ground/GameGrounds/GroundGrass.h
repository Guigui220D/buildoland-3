#pragma once

#include "../Ground.h"

#include <iostream>

class GroundGrass : public Ground
{
    public:
        GroundGrass();
        ~GroundGrass();

        inline bool hasTextureBleedings() const override { return true; }
        inline uint32_t getBleedingForNeighborGrounds(GroundInfo info) const override { return 4; }

        virtual sf::VertexArray getSurfaceDetails(GroundInfo info) const override
        {
            sf::VertexArray va;

            uint32_t flower = getRandomInt(info) % 10;

            if (flower == 0)
            {
                TextQuad three_flowers = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(8, getRandomInt(info) % 4), info.getPos());
                for (int i = 0; i < 4; i++)
                    va.append(three_flowers.verts[i]);
            }
            else if (flower == 1)
            {
                TextQuad two_flowers = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(9, getRandomInt(info) % 4), info.getPos());
                for (int i = 0; i < 4; i++)
                    va.append(two_flowers.verts[i]);
            }

            addNeighborsBleeding(info, va);

            return va;
        }

    protected:

    private:
};

