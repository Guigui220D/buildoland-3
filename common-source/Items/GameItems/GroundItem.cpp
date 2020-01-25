#include "GroundItem.h"

GroundItem::GroundItem(Ground const * ground) :
    Item(ground->getName() + "_ground", ground->default_texture),
    ground(ground)
{

}

GroundItem::~GroundItem()
{
    //dtor
}
