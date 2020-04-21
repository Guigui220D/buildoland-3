#pragma once

#include "TileEntityCodes.h"

#include <SFML/System/Vector2.hpp>

#ifdef CLIENT_SIDE
namespace sf { class RenderTarget; }
#else

#endif // CLIENT_SIDE

class World;
class Chunk;
class ECCPacket;

class TileEntity
{
    public:
        TileEntity(Chunk& chunk, sf::Vector2i pos);
        virtual ~TileEntity();

        virtual inline unsigned short getTypeCode() const = 0;

        inline World& getWorld() const { return world; }
        inline Chunk& getChunk() const { return chunk; }

        inline sf::Vector2i getTilePos() const { return tile_pos; }
        sf::Vector2i getPosInChunk() const;

        /**
         * Updates this tile-entity
         * This function is meant to be overriden
         * @param delta : time since last update
         */
        virtual void update(float delta);

        #ifdef CLIENT_SIDE
        /**
         * Draw this tile-entity
         * @param target : the render target on which to draw
         */
        virtual void draw(sf::RenderTarget& target) const;
        /**
         * Draws more things that should be drawn over block tops
         * @param target : the render target on which to draw
         */
        virtual void drawAbove(sf::RenderTarget& target) const;

        virtual bool readTileEntityPacket(ECCPacket& packet);
        #else

        #endif

    protected:

    private:
        World& world;
        Chunk& chunk;

        sf::Vector2i tile_pos;
};
