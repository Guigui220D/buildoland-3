#pragma once

#include "Item.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>

#include "../Blocks/GameBlocks.h"
#include "../Grounds/GameGrounds.h"

#ifdef CLIENT_SIDE
class LanguageManager;
#endif // CLIENT_SIDE

class ItemsRegister
{
    public:
        #ifdef CLIENT_SIDE
        ItemsRegister(LanguageManager& language);
        #else
        ItemsRegister();
        #endif // CLIENT_SIDE

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
        static Item const * const NULL_ITEM;
        static Item const * const BALL;
        static Item const * const SHOVEL;
        static Item const * const SPYGLASS;
        static Item const * const EMPTY_BUCKET;
        static Item const * const WATER_BUCKET;

    private:
        std::vector<Item const *> items;
        std::unordered_map<std::string, uint16_t> names;

        /**
         * Adds an item to the item list and its name
         * Fails if theres more items than the id type allows or if the name is already used
         * @param item : the item to add, must be a constant pointer to a constant item inside of THIS class
         */
        void addItem(Item const * item);

        #ifdef CLIENT_SIDE
        LanguageManager& language;
        #endif // CLIENT_SIDE
};
