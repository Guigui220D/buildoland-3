#include "ItemStack.h"

#include <limits>
#include <iostream>

#include "ItemsRegister.h"

ItemStack::ItemStack(Item const * item, uint8_t amount) :
    item(item),
    amount(amount)
{}

ItemStack::ItemStack() :
    item(ItemsRegister::NULL_ITEM),
    amount(0)
{}

ItemStack::ItemStack(uint32_t integer, const ItemsRegister& reg) :
    item(reg.getItemByID(integer & 0xFFFF)),
    amount((integer & 0xFF0000) >> 16)
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

bool ItemStack::add(ItemStack& other)
{
    if (!operator bool())
    {
        swap(other);
        return true;
    }

    if (item != other.item)
        return false;

    int space = std::numeric_limits<uint8_t>::max() - amount;

    if (other.amount > space)
    {
        other.amount -= space;
        amount = std::numeric_limits<uint8_t>::max();
        return false;
    }
    else
    {
        amount += other.amount;
        other.amount = 0;
        return true;
    }
}

void ItemStack::reset()
{
    amount = 0;
    item = ItemsRegister::NULL_ITEM;
}

ItemStack::operator bool() const
{
    return (amount != 0 && item != ItemsRegister::NULL_ITEM);
}
