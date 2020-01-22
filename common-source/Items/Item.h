#pragma once

#include <string>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../Networking/NetworkingCodes.h"

class World;
class ItemStack;
class Player;

class Item
{
    friend class ItemsRegister;
    public:
        Item(const std::string name);
        virtual ~Item();

        /**
         * Get the name of this item
         * @return The name of this item
         */
        inline const std::string getName() const { return name; }
        /**
         * Get the id of this item
         * The id is determined when the item is added to the item manager
         * @return The id of this item
         */
        inline uint16_t getId() const { return id; }

        /**
         * Use the item
         * @param stack : a reference to the item stack this has been used from
         * @param world : a reference to the world, in case it has to be modified
         * @param click_pos : the position that has been clicked in the world
         */
        void useItem(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    protected:
        /**
         * Use the item
         * Called by useItem, to be overriden
         */
        virtual void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    private:
        std::string name;
        mutable uint16_t id;
};
