#pragma once

#include "ItemStack.h"

#include <array>

class Entity;

class Inventory
{
    public:
        Inventory(Entity const * owner);
        virtual ~Inventory();

        std::array<ItemStack, 24> contents;

        void describe() const;

    protected:

    private:
        Entity const * owner;
};
