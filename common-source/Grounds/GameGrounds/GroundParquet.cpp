#include "GroundParquet.h"

#include "../GameGrounds.h"

GroundParquet::GroundParquet() :
    Ground("wood", true, 5)
{
    //ctor
}

GroundParquet::~GroundParquet()
{
    //dtor
}

#ifdef CLIENT_SIDE
bool GroundParquet::acceptsTextureBleedings(TileReference info, const Ground* other) const
{
    return
    (other == GameGrounds::WATER && other->getId() != getId()) ||
    (other->hasTextureBleedings() && (other->getId() > getId()) && !other->isNatural());
}
#endif // CLIENT_SIDE
