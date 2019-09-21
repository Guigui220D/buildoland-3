#include "PhysicsEntity.h"

#include "../World/World.h"
#include "../Block/Block.h"

#include <cmath>

PhysicsEntity::PhysicsEntity(World* world, unsigned int id, sf::Vector2f hitbox_size) :
    Entity(world, id),
    hitbox_size(hitbox_size)
{
    //ctor
}

PhysicsEntity::~PhysicsEntity()
{
    //dtor
}

bool PhysicsEntity::canBeHere(sf::Vector2f new_position) const
{
    sf::Vector2f points[4];
    points[0] = new_position - hitbox_size / 2.f;
    points[1] = new_position + sf::Vector2f(hitbox_size.x / 2.f, -hitbox_size.y / 2.f);
    points[2] = new_position + hitbox_size / 2.f;
    points[3] = new_position + sf::Vector2f(-hitbox_size.x / 2.f, hitbox_size.y / 2.f);

    for (int i = 0; i < 4; i++)
    {
        sf::Vector2i block_pos(points[i].x, points[i].y);
        if (getWorld()->getBlock(block_pos)->isSolid(BlockInfo(getWorld(), block_pos)))
            return false;
    }

    return true;
}
