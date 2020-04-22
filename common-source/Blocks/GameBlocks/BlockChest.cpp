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
    std::vector<ItemStack> vec;
    vec.emplace_back(ItemsRegister::SPYGLASS);
    return vec;
}
#endif // CLIENT_SIDE
