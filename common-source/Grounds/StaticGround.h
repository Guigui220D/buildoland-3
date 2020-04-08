#pragma once

#include "Ground.h"

class StaticGround : public Ground
{
    public:
        StaticGround(const std::string name, bool should_have_item, uint32_t default_texture);
        virtual ~StaticGround();

        #ifdef CLIENT_SIDE
        inline bool acceptsTextureBleedings(TileReference info, const Ground* other) const override { return false; }
        inline uint8_t getTextureRotation(TileReference info) const override { return 0; }
        #endif

    protected:

    private:
};
