#pragma once

#include "Item.h"

class ItemStack
{
    public:
        //Itemstack should be non copyable
        ItemStack& operator=(const ItemStack&) = delete;

        ItemStack(uint16_t item_id, uint8_t amount = 1);
        ItemStack();

        ~ItemStack();

        void swap(ItemStack& other);

        ItemStack takeSome(uint8_t how_much = 1);
        ItemStack takeHalf();

        inline uint8_t getAmount() const { return amount; }
        inline uint16_t getItemID() const { return item; }

    private:
        uint16_t item;
        uint8_t amount;
};
