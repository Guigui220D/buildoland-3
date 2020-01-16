#include "ItemsRegister.h"

#include <limits>
#include <iostream>

/*
Block const * const GameBlocks::AIR             = new BlockAir();
Block const * const GameBlocks::ERROR           = new BlockError();
Block const * const GameBlocks::STONE           = new BlockStone();
Block const * const GameBlocks::STONE_BRICKS    = new BlockStoneBricks;
Block const * const GameBlocks::IRON            = new BlockIron();
Block const * const GameBlocks::GOLD            = new BlockGold();
*/

ItemsRegister::ItemsRegister()
{
    //ctor
}

ItemsRegister::~ItemsRegister()
{
    //dtor
}

void ItemsRegister::initItems()
{
    names.clear();
    items.clear();
    //Add all items here
    /*
    addBlock(AIR);
    addBlock(ERROR);

    addBlock(STONE);
    addBlock(STONE_BRICKS);
    addBlock(IRON);
    addBlock(GOLD);
    */
}

void ItemsRegister::addItem(Item const * item)
{
    std::string name = item->getName();
    //Fail if the name is already used
    assert(names.find(name) == names.end());
    //Fail if theres more blocks than the id system can allow (very unlikely)
    assert(items.size() < std::numeric_limits<uint16_t>::max() - 1);
    uint16_t id = (uint16_t)items.size();

    names.emplace(name, id);
    items.push_back(item);
    item->id = id;

    std::cout << "Added item \"" << name << "\" with id " << id << std::endl;
}

Item const * ItemsRegister::getItemByID(unsigned short id) const
{
    if (id >= items.size())
        return /*ERROR*/nullptr;    //TEMP

    return items.at(id);
}

Item const * ItemsRegister::getItemByName(const std::string name) const
{
    auto ptr = names.find(name);

    if (ptr != names.end())
        return /*ERROR*/nullptr;    //TEMP

    uint16_t id = ptr->second;
    assert(id < items.size());

    return items.at(id);
}
