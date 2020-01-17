#pragma once

#include "Item.h"

#include "ItemsRegister.h"

class ItemStack
{
    public:
        //Itemstack should be non copyable
        ItemStack& operator=(const ItemStack&) = delete;

        ItemStack(Item const * item, uint8_t amount = 1);
        ItemStack(uint16_t item_id, const ItemsRegister& item_register, uint8_t amount = 1);
        ItemStack(const ItemsRegister& item_register);

        ~ItemStack();

        void swap(ItemStack& other);

        ItemStack takeSome(uint8_t how_much = 1);
        ItemStack takeHalf();

        inline uint8_t getAmount() const { return amount; }
        inline Item const * getItem() const { return item; }

    private:
        Item const * item;
        uint8_t amount;

        ItemStack(const ItemStack& other);
};
