#pragma once

#include "Entity.h"

#include "TileEntityCodes.h"

class Chunk;

class TileEntity : public Entity
{
    public:
        inline unsigned short getEntityCode() const override { return Entities::TileEntity; };
        virtual inline unsigned short getTileEntityCode() const { return TileEntities::None; };

        TileEntity(World& world, unsigned int id, sf::Vector2i tile_pos);
        virtual ~TileEntity();

        inline sf::Vector2i getTilePos() const { return tile_pos; }

        void update(float delta) override;

        #ifdef CLIENT_SIDE
        inline void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) override {}
        #else
        void makeNewEntityPacket(sf::Packet& packet) const override;
        #endif // CLIENT_SIDE

        inline bool isTileEntity() const override { return true; }

        inline bool isReady() const { return ready; }
        void assignChunk(Chunk* chunk_ptr);

    protected:
        Chunk* chunk;
        const sf::Vector2i tile_pos;
        const sf::Vector2i chunk_pos;

        virtual void teUpdate(float delta);

        bool ready;

    private:
};
