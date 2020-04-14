#pragma once

#include "../Item.h"

class EmptyBucketItem : public Item
{
    public:
        EmptyBucketItem();
        virtual ~EmptyBucketItem();

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    protected:

    private:
};

class WaterBucketItem : public Item
{
    public:
        WaterBucketItem();
        virtual ~WaterBucketItem();

        void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    protected:

    private:
};
