#pragma once

#include "../Block.h"

class BlockBoulder : public Block
{
    public:
        BlockBoulder();
        virtual ~BlockBoulder();

    protected:
        #ifdef CLIENT_SIDE
        inline bool occults(BlockInfo info) const override { return false; }
        inline uint32_t getSideTextureNumber(BlockInfo info) const override { return 36; }
        inline bool darkenSide() const override { return false; }
        #endif

        //inline unsigned short getTileEntityCode() const override { return TileEntities::TestTileEntity; }
    private:
};

