#include "BlockChest.h"

#include "../../Items/ItemsRegister.h"

BlockChest::BlockChest() :
    Block("chest", true, 8)
{
    //ctor
}

BlockChest::~BlockChest()
{
    //dtor
}

#ifndef CLIENT_SIDE
std::vector<ItemStack> BlockChest::getDrops() const
{
    return { ItemStack(ItemsRegister::SPYGLASS) };
}
#endif // CLIENT_SIDE
