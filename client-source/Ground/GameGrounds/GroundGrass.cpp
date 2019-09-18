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

    addNeighborsBleeding(info, va, frame);

    if (!(getRandomInt(info) % 6))
    {
        TextQuad detail = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(8 + (getRandomInt(info, 2) % 5), getRandomInt(info, 1) % 4), info.getPos());
        for (int i = 0; i < 4; i++)
            va.append(detail.verts[i]);
    }

    return va;
}
