#include "GroundError.h"

GroundError::GroundError() :
    Ground("error", false, 0)
{
    //ctor
}

GroundError::~GroundError()
{
    //dtor
}

#ifdef CLIENT_SIDE
bool GroundError::acceptsTextureBleedings(GroundInfo info, const Ground* other) const
{
    return false;
}
#endif
