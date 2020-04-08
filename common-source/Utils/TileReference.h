#pragma once

#include <SFML/System/Vector2.hpp>

class World;
class Block;
class Ground;

class TileReference
{
    public:
        TileReference(sf::Vector2i position, World& world);
        ~TileReference();

        operator const Block*() const;
        void operator=(const Block* block);

        const Block* getBlock() const;
        void setBlock(const Block* block);

        const Ground* getGround() const;
        void setGround(const Ground* ground);

        inline TileReference getRelative(sf::Vector2i rpos) const { return TileReference(pos + rpos, world); }

        inline sf::Vector2i getPos() { return pos; }
        inline World& getWorld() { return world; }

    private:
        sf::Vector2i pos;
        World& world;
};
