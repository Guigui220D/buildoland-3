#include "LivingEntity.h"

#include <cmath>
#include <iostream>

#ifdef CLIENT_SIDE

#else
    #include <SFML/Network.hpp>
    #include "../Networking/NetworkingCodes.h"
    #include "../../server-source/World/World.h"
    #include "../../server-source/Server/Server.h"
#endif // CLIENT_SIDE

#ifdef CLIENT_SIDE
LivingEntity::LivingEntity(World& world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed),
    animation(32, 3, .15f)
{
    animation.addAnimation({ 1 });
    for (unsigned int i = 0; i < 8; i++)
    {
        unsigned int j = i * 3;
        animation.addAnimation({ j + 0, j + 1, j + 2, j + 1 });
    }

}
#else
LivingEntity::LivingEntity(World& world, unsigned int id, sf::Vector2f hitbox_size, float speed) :
    PhysicsEntity(world, id, hitbox_size),
    walking_speed(speed)
{

}
#endif // CLIENT_SIDE


LivingEntity::~LivingEntity()
{
    //dtor
}

#ifdef CLIENT_SIDE
bool LivingEntity::takePacket(sf::Packet& packet)
{
    int action_code; packet >> action_code;

    if (!packet)
    {
        std::cerr << "Entity packet was too short (reading entity action code)." << std::endl;
        return false;
    }

    switch (action_code)
    {
    case Walk:
        {
            sf::Vector2f new_pos, new_direction;

            packet >> new_direction.x;
            packet >> new_direction.y;
            packet >> new_pos.x;
            packet >> new_pos.y;

            position = new_pos;
            setWalkingDirection(new_direction);
        }
        return true;
    default:
        std::cerr << "Living entity action code unknown." << std::endl;
        return false;
    }
}

bool LivingEntity::takeNewEntityPacket(sf::Packet& packet)
{
    sf::Vector2f new_pos, new_direction;

    packet >> new_pos.x;
    packet >> new_pos.y;
    packet >> new_direction.x;
    packet >> new_direction.y;

    position = new_pos;
    setWalkingDirection(new_direction);

    return true;
}
#else
void LivingEntity::addInfoToNewEntityPacket(sf::Packet& packet) const
{
    packet << position.x;
    packet << position.y;

    packet << walking_direction.x;
    packet << walking_direction.y;
}
#endif

void LivingEntity::setWalkingDirection(sf::Vector2f new_direction)
{
    walking_direction = new_direction;

    #ifndef CLIENT_SIDE
        sf::Packet packet;
        //Codes
        packet << Networking::StoC::EntityAction;
        packet << EntityActions::StoC::EntityAction;
        packet << getId();
        packet << Walk;
        //Direction
        packet << new_direction.x;
        packet << new_direction.y;
        //Position
        packet << position.x;
        packet << position.y;

        send(packet);
    #endif // SERVER_SIDE
}

void LivingEntity::walk(float delta)
{

    if (walking_direction == sf::Vector2f(0.f, 0.f))
    {
        #ifdef CLIENT_SIDE
        animation.selectAnimation(0);
        #endif
        return;
    }

    #ifdef CLIENT_SIDE
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
