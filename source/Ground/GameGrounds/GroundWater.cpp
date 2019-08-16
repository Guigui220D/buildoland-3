#include "GroundWater.h"

GroundWater::GroundWater() :
    Ground("water", 4)
{
    //ctor
}

GroundWater::~GroundWater()
{
    //dtor
}

sf::VertexArray GroundWater::getSurfaceDetails(GroundInfo info, int frame) const
{
    sf::VertexArray va;

    TextQuad detail;

    switch (frame)
    {
    case 0:
        detail = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(10, 0), info.getPos());
        break;
    case 1:
    case 3:
        detail = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(11, 0), info.getPos());
        break;
    case 2:
        detail = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(12, 0), info.getPos());
        break;
    }

    for (int i = 0; i < 4; i++)
            va.append(detail.verts[i]);

    addNeighborsBleeding(info, va);

    return va;
}
