#pragma once

#include <string>

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

    protected:

    private:
        std::string name;
        mutable uint16_t id;
};
