#pragma once

#include "../Block.h"

class BlockChest : public Block
{
    public:
        BlockChest();
        virtual ~BlockChest();

        #ifdef CLIENT_SIDE
        inline bool occults(TileReference info) const override { return false; }
        inline uint32_t getTopTextureNumber(TileReference info) const override { return 9; }
        #else
        virtual std::vector<ItemStack> getDrops() const;
        #endif

    protected:

    private:
};
