#pragma once

#include <SFML/System.hpp>

class World;
class GameBlocks;
class GameGrounds;

class GroundInfo
{
    public:
        GroundInfo(World* world, sf::Vector2i ground_pos) :
            world(world),
            ground_pos(ground_pos)
        { }

        inline World* getWorld() const { return world; }
        inline sf::Vector2i getPos() const { return ground_pos; }

        const GameGrounds& getGroundsManager() const;
        const GameBlocks& getBlocksManager() const;

    private:
        World* world;
        sf::Vector2i ground_pos;
};
