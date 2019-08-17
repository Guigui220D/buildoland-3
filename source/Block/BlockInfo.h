#pragma once

#include <SFML/System.hpp>

class World;
class GameBlocks;

class BlockInfo
{
    public:
        BlockInfo(World* world, sf::Vector2i block_pos) :
            world(world),
            block_pos(block_pos)
        { }

        inline World* getWorld() const { return world; }
        inline sf::Vector2i getPos() const { return block_pos; }

        const GameBlocks& getBlocksManager() const;

    private:
        World* world;
        sf::Vector2i block_pos;
};
