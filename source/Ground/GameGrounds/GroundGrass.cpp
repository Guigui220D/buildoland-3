#include "GroundGrass.h"

GroundGrass::GroundGrass() :
    Ground("grass", 3)
{
    //ctor
}

GroundGrass::~GroundGrass()
{
    //dtor
}

sf::VertexArray GroundGrass::getSurfaceDetails(GroundInfo info, int frame) const
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
