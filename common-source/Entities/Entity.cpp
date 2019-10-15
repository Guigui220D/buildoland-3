#include "Entity.h"



#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
#else
    #include "../Networking/NetworkingCodes.h"
    #include "../../server-source/World/World.h"
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

#ifndef CLIENT_SIDE
void Entity::send(sf::Packet& packet)
{
    getWorld()->sendToSubscribers(packet, chunk_on);
}
#endif // CLIENT_SIDE

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

        makeNewEntityPacket(enter);

        getWorld()->sendToSubscribersWithException(enter, new_chunk, old_chunk);
    }
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
bool Entity::takeNewEntityPacket(sf::Packet& packet) { return true; }
#else
void Entity::makeNewEntityPacket(sf::Packet& packet) const
{
    packet.clear();

    packet << (unsigned short)Networking::StoC::EntityAction;
    packet << (unsigned short)EntityActions::StoC::AddEntity;
    packet << (unsigned short)getEntityCode();
    packet << getId();

    addInfoToNewEntityPacket(packet);
}

void Entity::addInfoToNewEntityPacket(sf::Packet& packet) const {}
#endif // CLIENT_SIDE
