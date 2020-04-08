#pragma once

#include <SFML/System/Vector2.hpp>

class World;
class Block;
class Ground;
class GameBlocks;
class GameGrounds;

class TileReference
{
    public:
        TileReference(sf::Vector2i position, World& world);
        ~TileReference();

        /**
            @warning Be careful with this! for optimization, getBlock will fetch the block in the world once. Do NOT keep a block reference for a period of time in which the block might be changed by something else.
        */
        const Block* getBlock() const;
        void setBlock(const Block* block);

        /**
            @warning Be careful with this! for optimization, getBlock will fetch the block in the world once. Do NOT keep a block reference for a period of time in which the block might be changed by something else.
        */
        const Ground* getGround() const;
        void setGround(const Ground* ground);

        inline TileReference getRelative(sf::Vector2i rpos) const { return TileReference(pos + rpos, world); }

        inline sf::Vector2i getPos() { return pos; }
        inline World& getWorld() { return world; }

        const GameBlocks& getBlocksManager() const;
        const GameGrounds& getGroundsManager() const;

    private:
        sf::Vector2i pos;
        World& world;

        mutable const Block* known_block;
        mutable const Ground* known_ground;
};
