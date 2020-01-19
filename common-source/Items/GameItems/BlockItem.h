#pragma once

#include "../Item.h"

#include "../../Blocks/Block.h"

class BlockItem : public Item
{
    public:
        BlockItem(Block const * block);
        virtual ~BlockItem();

        inline Block const * getBlock() { return block; }

        #ifndef CLIENT_SIDE
        void use(ItemStack& stack, World& world, sf::Vector2i click_pos) const;
        #endif // CLIENT_SIDE

    protected:
        Block const * const block;

    private:
};
