#include "BallItem.h"

#include "../ItemStack.h"

BallItem::BallItem() :
    Item("ball")
{

}

BallItem::~BallItem()
{
    //dtor
}

#ifndef CLIENT_SIDE
void BallItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos) const
{
    if (stack.getAmount())
        stack.takeSome(1);
}
#endif // CLIENT_SIDE



