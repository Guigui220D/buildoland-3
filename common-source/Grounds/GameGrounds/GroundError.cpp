#include "GroundError.h"

GroundError::GroundError(const std::string name) :
    Ground(name, false, 0)
{
    //ctor
}

GroundError::~GroundError()
{
    //dtor
}

#ifdef CLIENT_SIDE
bool GroundError::acceptsTextureBleedings(TileReference info, const Ground* other) const
{
    return false;
}
#endif
