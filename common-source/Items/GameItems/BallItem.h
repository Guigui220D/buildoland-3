#pragma once

#include "../Item.h"

class BallItem : public Item
{
    public:
        BallItem();
        virtual ~BallItem();

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    protected:

    private:
};
