#pragma once

#include <SFML/System.hpp>

#include "Chunk.h"

class Block;
class Ground;

class Structure
{
    public:
        Structure();
        virtual ~Structure();

        inline void setCenter(sf::Vector2i center) { pos = center; }
        inline void setRotation(int rotation) { if (rotation < 0) rotation = -rotation; rota = rotation % 4; }
        inline void setChunk(sf::Vector2i position) { chunk = position; }

        inline sf::Vector2i getRealBlockPos(sf::Vector2i chunk, sf::Vector2i tile) const { return chunk * Chunk::CHUNK_SIZE + tile; }

        virtual const Block* getBlock(sf::Vector2i block_in_chunk) const = 0;
        virtual const Ground* getGround(sf::Vector2i block_in_chunk) const = 0;

    protected:
        sf::Vector2i pos;
        int rota;

        sf::Vector2i chunk;

    private:
};
