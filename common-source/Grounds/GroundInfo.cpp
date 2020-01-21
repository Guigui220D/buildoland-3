#include "GroundInfo.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
#else
    #include "../../server-source/World/World.h"
#endif


const GameGrounds& GroundInfo::getGroundsManager() const
{
    return world.getGroundsManager();
}
