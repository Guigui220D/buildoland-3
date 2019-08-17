#include "GroundInfo.h"

#include "../World/World.h"

const GameGrounds& GroundInfo::getGroundsManager() const
{
    return world->getGroundsManager();
}
