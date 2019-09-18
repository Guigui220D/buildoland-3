#pragma once

#include "../Block.h"

class BlockTree : public Block
{
    public:
        BlockTree();
        virtual ~BlockTree();

        inline bool occults(BlockInfo info) const override { return false; }
        inline bool alwaysVisible() const override { return true; }

        //TextQuad getTopVertices(BlockInfo info) const override;
        TextQuad getSideVertices(BlockInfo info) const override;

    protected:
        inline bool darkenSide() const override { return false; }

        inline uint32_t getTopTextureNumber(BlockInfo info) const override { return 1; }
        inline uint32_t getSideTextureNumber(BlockInfo info) const override { return 35; }

    private:
};

