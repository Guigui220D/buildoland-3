#pragma once

#include "Item.h"

class ItemsRegister;

class ItemStack
{
    public:
        //Itemstack should be non copyable
        ItemStack& operator=(const ItemStack&) = delete;
        ItemStack(const ItemStack&) = delete;
        ItemStack(ItemStack&& stack) noexcept = default;

        ItemStack(Item const * item, uint8_t amount = 1);
        ItemStack();
        ItemStack(uint32_t integer, const ItemsRegister& reg);

        ~ItemStack();

        /**
         * Swaps both stacks
         * @param other : a reference to the other stack
         */
        void swap(ItemStack& other);

        /**
         * Takes some items from that stack and returns a new one
         * @param how_much : How many items should be taken
         * @return A new item stack
         */
        ItemStack takeSome(uint8_t how_much = 1);
        /**
         * Takes half of this stak to make a new one
         * @return A new item stack
         */
        ItemStack takeHalf();

        /**
         * Adds some items to the stack from an other one
         * @param other : the other stack that we should try to add
         * @return True if the other stack was emptied
         */
        bool add(ItemStack& other);

        /**
         * Sets the amount and item to 0
         */
        void reset();

        operator bool() const;

        inline uint8_t getAmount() const { return amount; }
        inline Item const * getItem() const { return item; }

        inline uint32_t getInt() const { return item->getId() | amount << 16; }

    private:
        Item const * item;
        uint8_t amount;
};
