#include "GroundItem.h"

GroundItem::GroundItem(Ground const * ground) :
    #ifdef CLIENT_SIDE
    Item(ground->getName() + "_ground", ground->default_texture),
    #else
    Item(ground->getName() + "_ground", 0),
    #endif // CLIENT_SIDE
    ground(ground)
{

}

GroundItem::~GroundItem()
{
    //dtor
}
