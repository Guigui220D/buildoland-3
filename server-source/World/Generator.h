#pragma once

class Chunk;

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

        inline int getSeed() const { return seed; }

    protected:

        const int seed;

    private:
};
