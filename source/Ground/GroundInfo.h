#pragma once

#include <SFML/System.hpp>

class World;

class GroundInfo
{
    public:
        GroundInfo(World* world, sf::Vector2i ground_pos) :
            world(world),
            ground_pos(ground_pos)
        { }

        inline World* getWorld() const { return world; }
        inline sf::Vector2i getPos() const { return ground_pos; }

    private:
        World* world;
        sf::Vector2i ground_pos;
};
