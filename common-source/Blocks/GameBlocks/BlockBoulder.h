#pragma once

#include "../Block.h"

class BlockBoulder : public Block
{
    public:
        BlockBoulder(const std::string name, uint32_t top_texture, uint32_t side_texture);
        virtual ~BlockBoulder();

    protected:
        #ifdef CLIENT_SIDE
        inline bool occults(BlockInfo info) const override { return false; }
        inline uint32_t getSideTextureNumber(BlockInfo info) const override { return side_texture; }
        inline bool darkenSide() const override { return false; }
        #endif
    private:
        uint32_t side_texture;
};

