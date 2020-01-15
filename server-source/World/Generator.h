#pragma once

#include "Chunk.h"

#include "../Ground/GameGrounds.h"
#include "../Block/GameBlocks.h"

class Generator
{
    public:
        Generator();
        virtual ~Generator();

        /**
        * Generates a chunk
        * This function should only call setBlock and setGround on the chunk
        * @param chunk : a pointer to the chunk to populate
        */
        virtual void generateChunk(Chunk* chunk) = 0;

    protected:

    private:
};
