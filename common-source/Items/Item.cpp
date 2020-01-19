#include "Item.h"

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
void Item::use(ItemStack& stack, World& world, sf::Vector2i click_pos) const
{}
#endif
