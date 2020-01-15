#include "EmptyGenerator.h"

EmptyGenerator::EmptyGenerator()
{
    //ctor
}

EmptyGenerator::~EmptyGenerator()
{
    //dtor
}

void EmptyGenerator::generateChunk(Chunk* chunk)
{
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
    for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
    {
        chunk->setGround(x, y, GameGrounds::SAND);
    }

    chunk->setGround(0, 0, GameGrounds::GRASS);
}
