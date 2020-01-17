#pragma once

#include "../Item.h"

#include "../../Blocks/Block.h"

class BlockItem : public Item
{
    public:
        BlockItem(Block const * block);
        virtual ~BlockItem();

        inline Block const * getBlock() { return block; }

    protected:
        Block const * const block;

    private:
};
