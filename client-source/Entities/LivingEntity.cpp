#include "LivingEntity.h"

#include <cmath>

LivingEntity::LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed),
    animation(.2f)
{
    animation.addAnimation({ 0 });
    animation.addAnimation({ 3 });
    animation.addAnimation({ 6 });
    animation.addAnimation({ 9 });
    animation.addAnimation({ 0, 1, 0, 2 });
    animation.addAnimation({ 3, 4, 3, 5 });
    animation.addAnimation({ 6, 7, 6, 8 });
    animation.addAnimation({ 9, 10, 9, 11 });
}

LivingEntity::~LivingEntity()
{
    //dtor
}

void LivingEntity::walk(float delta)
{
    if (walking_direction == sf::Vector2f(0.f, 0.f))
    {
        animation.selectAnimation(0);
        return;
    }

    //Selecting a proper animation
    if (walking_direction.x > 0.f)
    {
        if (walking_direction.y == 0.f)
            animation.selectAnimation(6);
        if (walking_direction.y > 0.f)
            animation.selectAnimation(5);
        if (walking_direction.y < 0.f)
            animation.selectAnimation(7);
    }
    else if (walking_direction.x < 0.f)
    {
        if (walking_direction.y == 0.f)
            animation.selectAnimation(6);
        if (walking_direction.y > 0.f)
            animation.selectAnimation(7);
        if (walking_direction.y < 0.f)
            animation.selectAnimation(5);
    }
    else
        animation.selectAnimation(walking_direction.y == 0.f ? 0 : 4);

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
