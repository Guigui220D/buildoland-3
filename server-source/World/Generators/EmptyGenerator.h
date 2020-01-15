#pragma once

#include "../Generator.h"

class EmptyGenerator : public Generator
{
    public:
        EmptyGenerator();
        ~EmptyGenerator();

        void generateChunk(Chunk* chunk) override;

    protected:

    private:
};
