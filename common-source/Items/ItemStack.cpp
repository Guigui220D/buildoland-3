#include "ItemStack.h"

ItemStack::ItemStack(uint16_t item_id, uint8_t amount) :
    item(item_id),
    amount(amount)
{}

ItemStack::ItemStack() :
    item(0),
    amount(0)
{}

ItemStack::~ItemStack() {}

void ItemStack::swap(ItemStack& other)
{
    uint16_t tmpi = other.item;
    uint8_t tmpa = other.amount;

    other.item = item;
    other.amount = amount;

    item = tmpi;
    amount = tmpa;
}

ItemStack ItemStack::takeSome(uint8_t how_much)
{
    if (how_much > amount)
        how_much = amount;

    amount -= how_much;

    return ItemStack(item, how_much);
}

ItemStack ItemStack::takeHalf()
{
    uint8_t half = amount / 2;
    amount -= half;

    return ItemStack(item, half);
}
