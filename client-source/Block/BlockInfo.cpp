#include "BlockInfo.h"

#include "../World/World.h"

const GameBlocks& BlockInfo::getBlocksManager() const
{
    return world->getBlocksManager();
}
