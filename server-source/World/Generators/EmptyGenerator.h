#pragma once

#include "../Generator.h"

class EmptyGenerator : public Generator
{
    public:
        EmptyGenerator();
        ~EmptyGenerator();

        void generateChunk(Chunk* chunk, std::vector<Entity*>& new_entities) override;

    protected:

    private:
};
