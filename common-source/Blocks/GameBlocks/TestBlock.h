#pragma once

#include "../Block.h"

class TestBlock : public Block
{
    public:
        TestBlock();
        virtual ~TestBlock();

        virtual inline bool clientSideHasTE() const { return false; };
        virtual inline bool serverSideHasTE() const { return true; };
        virtual inline unsigned short getTileEntityCode() const { return TileEntities::TestTE; }

    protected:

    private:
};
