#pragma once

#include "Entity.h"

class Chunk;

class TileEntity : public Entity
{
    public:
        inline unsigned short getEntityCode() const override { return Entities::TileEntity; };

        TileEntity(World& world, unsigned int id, Chunk& chunk, sf::Vector2i tile_pos);
        virtual ~TileEntity();

        inline sf::Vector2i getTilePos() const { return tile_pos; }

        #ifdef CLIENT_SIDE
        inline void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) override {}
        #endif // CLIENT_SIDE

    protected:
        Chunk& chunk;
        sf::Vector2i tile_pos;

    private:
};
