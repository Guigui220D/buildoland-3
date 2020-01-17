#include "ItemStack.h"

ItemStack::ItemStack(Item const * item, uint8_t amount) :
    item(item),
    amount(amount)
{}

ItemStack::ItemStack(uint16_t item_id, const ItemsRegister& item_register, uint8_t amount) :
    item(item_register.getItemByID(item_id)),
    amount(amount)
{}

ItemStack::ItemStack(const ItemsRegister& item_register) :
    item(item_register.NULL_ITEM),
    amount(0)
{}

ItemStack::ItemStack(const ItemStack& other) :
    item(other.getItem()),
    amount(other.getAmount())
{}

ItemStack::~ItemStack() {}

void ItemStack::swap(ItemStack& other)
{
    Item const * tmpi = other.item;
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
