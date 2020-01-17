#include "BlockItem.h"

BlockItem::BlockItem(Block const * block) :
    Item(block->getName() + "_block"),
    block(block)
{
    //ctor
}

BlockItem::~BlockItem()
{
    //dtor
}
