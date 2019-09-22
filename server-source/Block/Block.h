#pragma once

#include <string>

#include "BlockInfo.h"

class Block
{
    friend class GameBlocks;

    public:

        Block(const std::string name);
        virtual ~Block();

        /**
         * To know whether an entity's hitbox can touch that box
         * @param info : Info about the block
         * @return True if the block can be touched
         */
        virtual inline bool isSolid(BlockInfo info) const { return true; }
        /**
         * Get the name of this ground
         * @return The name of this ground
         */
        inline const std::string getName() const { return name; }
        /**
         * Get the id of this ground
         * The id is "calculated" when the ground is added to the ground manager
         * @return The id of this ground
         */
        inline uint16_t getId() const { return id; }

    private:
        std::string name;
        mutable uint16_t id;
};
