#include "ItemsRegister.h"

#include <limits>
#include <iostream>

#include "GameItems/GroundItem.h"
#include "GameItems/BlockItem.h"

#include "GameItems/NullItem.h"
#include "GameItems/BallItem.h"

Item const * const ItemsRegister::NULL_ITEM = new NullItem();
Item const * const ItemsRegister::BALL      = new BallItem();

ItemsRegister::ItemsRegister()
{
    //ctor
}

ItemsRegister::~ItemsRegister()
{
    //dtor
}

void ItemsRegister::initItems(GameBlocks& blocks, GameGrounds& grounds)
{
    names.clear();
    items.clear();

    addItem(NULL_ITEM);
    //Add all items here
    for (Ground const * ground : grounds.grounds)
        if (ground->hasItem())
            addItem(new GroundItem(ground));

    for (Block const * block : blocks.blocks)
        if (block->hasItem())
        {
            Item const * item = new BlockItem(block);
            addItem(item);
            #ifndef CLIENT_SIDE
            block->drop = item;
            #endif
        }


    addItem(BALL);
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

Item const * ItemsRegister::getItemByID(uint16_t id) const
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
