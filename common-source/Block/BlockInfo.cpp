#include "BlockInfo.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
#else
    #include "../../server-source/World/World.h"
#endif

const GameBlocks& BlockInfo::getBlocksManager() const
{
    return world->getBlocksManager();
}
