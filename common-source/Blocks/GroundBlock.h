#pragma once

#include "Block.h"

class GroundBlock : public Block
{
    public:
        GroundBlock(const std::string name, bool should_have_item, uint32_t default_textures_begin, uint32_t default_textures_count);
        virtual ~GroundBlock();

        inline bool isSolid(BlockInfo info) const override { return false; }
        inline bool hasVolume(BlockInfo info) const override { return false; }

        #ifdef CLIENT_SIDE
        inline bool occults(BlockInfo info) const { return false; }

        TextQuad getTopVertices(BlockInfo info) const override;
        #endif

    protected:
        #ifdef CLIENT_SIDE
        inline uint32_t getTopTextureNumber(BlockInfo info) const override { return getDefaultTexture() + getRandomInt(info, 12) % default_textures_count; }
        #endif // CLIENT_SIDE

    private:
        const uint32_t default_textures_count;
};
