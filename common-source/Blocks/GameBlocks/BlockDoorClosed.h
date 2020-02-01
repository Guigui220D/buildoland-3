#pragma once

#include "../Block.h"

class BlockDoorClosed : public Block
{
    public:
        BlockDoorClosed();
        virtual ~BlockDoorClosed();

    protected:
        #ifdef CLIENT_SIDE
        inline bool occults(BlockInfo info) const override { return false; }
        inline uint32_t getSideTextureNumber(BlockInfo info) const override { return 37; }
        inline bool darkenSide() const override { return false; }
        #endif
    private:
};

