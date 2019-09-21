#pragma once

#include "../Block.h"

class BlockAir : public Block
{
    public:
        BlockAir();
        virtual ~BlockAir();

        inline bool isSolid(BlockInfo info) const override { return false; }
        inline bool hasVolume(BlockInfo info) const override { return false; }
        inline bool occults(BlockInfo info) const override { return false; }

    protected:

    private:
};

