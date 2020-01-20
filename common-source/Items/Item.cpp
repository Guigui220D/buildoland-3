#include "Item.h"

#include "ItemStack.h"

Item::Item(const std::string name) :
    name(name)
{
    //ctor
}

Item::~Item()
{
    //dtor
}

#ifndef CLIENT_SIDE
void Item::useItem(ItemStack& stack, World& world, sf::Vector2i click_pos) const
{
    if (stack.getAmount() == 0)
        stack.reset();

    stack.getItem()->use(stack, world, click_pos);
}

void Item::use(ItemStack& stack, World& world, sf::Vector2i click_pos) const
{}
#endif
