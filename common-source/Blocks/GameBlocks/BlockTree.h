#pragma once

#include "../Block.h"

class BlockTree : public Block
{
    public:
        BlockTree();
        virtual ~BlockTree();

        #ifdef CLIENT_SIDE
        inline bool occults(TileReference info) const override { return false; }
        inline uint32_t getTopTextureNumber(TileReference info) const override { return 15; }
        inline bool darkenSide() const override { return false; }
        #endif

        virtual inline bool clientSideHasTE() const { return true; };
        virtual inline unsigned short getTileEntityCode() const { return TileEntities::TreeTopEntity; }

    protected:

    private:
};
