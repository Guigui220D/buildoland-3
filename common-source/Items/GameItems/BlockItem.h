#pragma once

#include "../Item.h"

#include "../../Blocks/Block.h"

class BlockItem : public Item
{
    public:
        BlockItem(Block const * block);
        virtual ~BlockItem();

        inline Block const * getBlock() const { return block; }

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

        #ifdef CLIENT_SIDE
        virtual inline TextureSet getTexturesSet() const { return TextureSet::BlocksTextureSet; }
        #endif // CLIENT_SIDE

    protected:
        Block const * const block;

    private:
};
