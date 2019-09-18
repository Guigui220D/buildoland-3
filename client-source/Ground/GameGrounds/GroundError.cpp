#include "GroundError.h"

GroundError::GroundError() :
    Ground("error", 0)
{
    //ctor
}

GroundError::~GroundError()
{
    //dtor
}

bool GroundError::acceptsTextureBleedings(GroundInfo info, const Ground* other) const
{
    return false;
}
