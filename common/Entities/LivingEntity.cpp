#include "LivingEntity.h"

#include <cmath>

#ifdef CLIENT_SIDE
LivingEntity::LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed),
    animation(.07f)
{
    animation.addAnimation({ 0 });
    for (unsigned int i = 0; i < 8; i++)
    {
        unsigned int j = i * 5;
        animation.addAnimation({ j + 1, j + 2, j + 1, j + 0, j + 3, j + 4, j + 3, j + 0 });
    }

}
#else
LivingEntity::LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed)
{

}
#endif // CLIENT_SIDE


LivingEntity::~LivingEntity()
{
    //dtor
}

void LivingEntity::walk(float delta)
{
    #ifdef CLIENT_SIDE
    if (walking_direction == sf::Vector2f(0.f, 0.f))
    {
        animation.selectAnimation(0);
        return;
    }

    //Selecting a proper animation
    if (walking_direction.x > 0.f)
    {
        if (walking_direction.y == 0.f)
            animation.selectAnimation(3);
        if (walking_direction.y > 0.f)
            animation.selectAnimation(2);
        if (walking_direction.y < 0.f)
            animation.selectAnimation(4);
    }
    else if (walking_direction.x < 0.f)
    {
        if (walking_direction.y == 0.f)
            animation.selectAnimation(7);
        if (walking_direction.y > 0.f)
            animation.selectAnimation(8);
        if (walking_direction.y < 0.f)
            animation.selectAnimation(6);
    }
    else
        animation.selectAnimation(walking_direction.y > 0.f ? 1 : 5);
    #endif // CLIENT_SIDE

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
