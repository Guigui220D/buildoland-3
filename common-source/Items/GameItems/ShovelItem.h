#pragma once

#include "../Item.h"

class ShovelItem : public Item
{
    public:
        ShovelItem();
        virtual ~ShovelItem();

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    protected:

    private:
};
