#include "LivingEntity.h"

#include <cmath>

LivingEntity::LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed),
    animation(.2f)
{
    animation.addAnimation({ 1 });
    animation.addAnimation({ 1, 0, 1, 2 });
    animation.addAnimation({ 4, 3, 4, 5 });
    animation.addAnimation({ 7, 6, 7, 8 });
    animation.addAnimation({ 10, 9, 10, 11 });
}

LivingEntity::~LivingEntity()
{
    //dtor
}

void LivingEntity::walk(float delta)
{
    //Selecting a proper animation
    if (walking_direction == sf::Vector2f(0.f, 0.f))
    {
        animation.selectAnimation(0);
        return;
    }
    if (std::abs(walking_direction.x) > std::abs(walking_direction.y))
        animation.selectAnimation(walking_direction.x >= 0.f ? 3 : 2);
    else
        animation.selectAnimation(walking_direction.y >= 0.f ? 1 : 4);

    //Move
    float distance = walking_direction.x * walking_direction.x + walking_direction.y * walking_direction.y;
    distance = std::sqrt(distance);

    //We normalize, then multiply by speed and delta the vector
    sf::Vector2f movement = (walking_direction / distance) * walking_speed * delta;

    if (canBeHere(position + movement))
    {
        position += movement;
    }
    else
    {
        float tmp = movement.x;
        movement.x = 0.f;
        if (canBeHere(position + movement))
        {
            position += movement;
        }
        else
        {
            movement.x = tmp;
            movement.y = 0.f;

            if (canBeHere(position + movement))
                position += movement;
        }
    }
}
