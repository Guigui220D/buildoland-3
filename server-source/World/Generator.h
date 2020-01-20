#pragma once

#include "Chunk.h"

#include "../../common-source/Grounds/GameGrounds.h"
#include "../../common-source/Blocks/GameBlocks.h"


class Generator
{
    public:
        Generator(int seed);
        virtual ~Generator();

        /**
        * The constructor is called before blocks and grounds are initialized but init after
        */
        virtual void init();

        /**
        * Generates a chunk
        * This function should only call setBlock and setGround on the chunk
        * @param chunk : a pointer to the chunk to populate
        */
        virtual void generateChunk(Chunk* chunk) = 0;

    protected:

        const int seed;

    private:
};