#pragma once

#include "Item.h"

#include <vector>
#include <map>
#include <string>
#include <assert.h>

#include "../Blocks/GameBlocks.h"
#include "../Grounds/GameGrounds.h"

class ItemsRegister
{
    public:
        ItemsRegister();
        ~ItemsRegister();

        /**
         * Inits the item list (reinits if called again)
         * @param blocks : The block manager, to import block items from
         * @param grounds : The ground manager, to import block items from
         */
        void initItems(GameBlocks& blocks, GameGrounds& grounds);

        /**
         * Gets a pointer to a item with its ID
         * @param id : the id of the item you want
         * @return The pointer to the item
         */
        Item const * getItemByID(uint16_t id) const;
        /**
         * Gets a pointer to an item using its name
         * @param name : the name of the item
         * @return The pointer to the item
         */
        Item const * getItemByName(const std::string name) const;

        //Put items here
        static Item const * const BALL;

    private:
        std::vector<Item const *> items;
        std::map<std::string, uint16_t> names;

        /**
         * Adds an item to the item list and its name
         * Fails if theres more items than the id type allows or if the name is already used
         * @param item : the item to add, must be a constant pointer to a constant item inside of THIS class
         */
        void addItem(Item const * item);
};
