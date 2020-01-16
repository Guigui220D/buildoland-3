#include "GroundItem.h"

GroundItem::GroundItem(Ground const * ground) :
    Item(ground->getName() + "_item"),
    ground(ground)
{

}

GroundItem::~GroundItem()
{
    //dtor
}
