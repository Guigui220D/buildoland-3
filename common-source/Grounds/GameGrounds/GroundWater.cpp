#include "GroundWater.h"

GroundWater::GroundWater() :
    Ground("water", true, 2)
{
    //ctor
}

GroundWater::~GroundWater()
{
    //dtor
}

#ifdef CLIENT_SIDE
sf::VertexArray GroundWater::getSurfaceDetails(GroundInfo info, int frame) const
{
    sf::VertexArray va;

    TextQuad detail1, detail2;

    if (frame == 3)
        frame = 1;

    addNeighborsBleeding(info, va, frame);
    detail1 = utils::getSquare(Ground::tilesetHelperDetails.getFourVertices(16 + frame, 0), info.getPos());

    for (int i = 0; i < 4; i++)
            va.append(detail1.verts[i]);

    return va;
}
#endif
