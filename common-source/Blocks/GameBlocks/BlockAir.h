#pragma once

#include "../Block.h"

class BlockAir : public Block
{
    public:
        BlockAir();
        virtual ~BlockAir();

        inline bool isSolid(TileReference info) const override { return false; }
        inline bool hasVolume(TileReference info) const override { return false; }

        #ifdef CLIENT_SIDE
        inline bool occults(TileReference info) const override { return false; }
        #endif

    protected:

    private:
};

