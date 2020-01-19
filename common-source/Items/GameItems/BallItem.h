#pragma once

#include "../Item.h"

class BallItem : public Item
{
    public:
        BallItem();
        virtual ~BallItem();

        #ifndef CLIENT_SIDE
        void use(ItemStack& stack, World& world, sf::Vector2i click_pos) const;
        #endif // CLIENT_SIDE

    protected:

    private:
};
