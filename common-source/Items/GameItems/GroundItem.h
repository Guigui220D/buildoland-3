#pragma once

#include "../Item.h"

#include "../../Grounds/Ground.h"

class GroundItem : public Item
{
    public:
        GroundItem(Ground const * ground);
        virtual ~GroundItem();

        inline Ground const * getGround() { return ground; }

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

        #ifdef CLIENT_SIDE
        virtual inline TextureSet getTexturesSet() const { return TextureSet::GroundsTextureSet; }
        #endif // CLIENT_SIDE

    protected:
        Ground const * const ground;

    private:
};
