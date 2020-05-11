#include "ItemStack.h"

#include <limits>

#include "ItemsRegister.h"

#ifndef CLIENT_SIDE
    #include "../../external/json/Json.hpp"
#endif // CLIENT_SIDE

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

#ifndef CLIENT_SIDE
ItemStack::ItemStack(nlohmann::json& json, const ItemsRegister& reg, bool& valid)
{
    valid = false;

    if (json.is_object())
    {
        valid = true;

        if (json["item"].is_string())
        {
            item = reg.getItemByName(json["item"].get<std::string>());
        }
        else
            valid = false;

        if (json["amount"].is_number())
            amount = json["amount"].get<uint8_t>();
        else
            valid = false;

        if (valid)
            valid = operator bool();
    }
}
#endif // CLIENT_SIDE

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
    if (!operator bool() || !other)
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

#ifndef CLIENT_SIDE
nlohmann::json* ItemStack::serializeToJson() const
{
    nlohmann::json* json = new nlohmann::json();
    (*json)["item"] = item->getName();
    (*json)["amount"] = amount;
    return json;
}
#endif // CLIENT_SIDE
