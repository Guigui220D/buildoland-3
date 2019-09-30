#include "Entity.h"

#include "../../server-source/World/World.h"

#ifndef CLIENT_SIDE
    #include "../Networking/NetworkingCodes.h"
#endif // CLIENT_SIDE

#include <iostream>

Entity::Entity(World* world, unsigned int id) :
    position(0.f, 0.f),
    id(id),
    world(world)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void Entity::updateBase(float delta)
{
    update(delta);

    sf::Vector2i co = getChunkOn();
    if (co != chunk_on)
    {
        onChunkChange(chunk_on, co);
        chunk_on = co;
    }
}

void Entity::update(float delta) {}

#ifdef CLIENT_SIDE
void Entity::draw(sf::RenderTarget& target) const {}
#endif // CLIENT_SIDE

sf::Vector2i Entity::getChunkOn() const
{
    return World::getChunkPosFromBlockPos(getBlockOn());
}

void Entity::onChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk)
{
    std::cout << "Entity " << getId() << " changed chunk. " << std::endl;

    #ifndef CLIENT_SIDE
    {
        sf::Packet leave;

        leave << (unsigned short)Networking::StoC::EntityAction;
        leave << (unsigned short)EntityActions::StoC::ForgetEntity;
        leave << getId();

        getWorld()->sendToSubscribersWithException(leave, old_chunk, new_chunk);
    }
    {
        sf::Packet enter;

        enter << (unsigned short)Networking::StoC::EntityAction;
        enter << (unsigned short)EntityActions::StoC::AddEntity;
        enter << (unsigned short)getEntityCode();
        enter << getId();

        getWorld()->sendToSubscribersWithException(enter, new_chunk, old_chunk);
    }
    #endif // CLIENT_SIDE
}
