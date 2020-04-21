#pragma once

#include "../Block.h"

class BlockGlass : public Block
{
    public:
        BlockGlass();
        virtual ~BlockGlass();

        #ifdef CLIENT_SIDE
        inline bool occults(TileReference info) const override { return false; }
        #endif

    protected:

    private:
};

