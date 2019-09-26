#pragma once

#include "../Block.h"

class BlockAir : public Block
{
    public:
        BlockAir();
        virtual ~BlockAir();

        inline bool isSolid(BlockInfo info) const override { return false; }

    protected:

    private:
};

