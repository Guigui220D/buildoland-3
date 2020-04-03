#include "Entity.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
#else
    #include "../../server-source/World/World.h"

    #include "../../server-source/Packets/EntityChunkLeavePacket.h"
#endif // CLIENT_SIDE

#include <iostream>

Entity::Entity(World& world, unsigned int id) :
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
void Entity::send(ECCPacket& packet)
{
    getWorld().sendToSubscribers(packet, chunk_on);
}
#endif // CLIENT_SIDE

#ifdef CLIENT_SIDE
void Entity::draw(sf::RenderTarget& target) const {}
void Entity::drawAbove(sf::RenderTarget& target) const {}
#endif // CLIENT_SIDE

sf::Vector2i Entity::getChunkOn() const
{
    return World::getChunkPosFromBlockPos(getBlockOn());
}

void Entity::onChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk)
{
    #ifndef CLIENT_SIDE
    {
        EntityChunkLeavePacket leave(getId());
        getWorld().sendToSubscribersWithException(leave, old_chunk, new_chunk);
    }
    {
        ECCPacket enter;
        //std::cout << "New entity spawn packet, for " << getId() << " (chunkchange)" << std::endl;
        makeNewEntityPacket(enter);

        getWorld().sendToSubscribersWithException(enter, new_chunk, old_chunk);
    }
    #else
    moreOnChunkChange(old_chunk, new_chunk);
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
void Entity::moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) {}

bool Entity::takePacket(ECCPacket& packet) { return true; }

bool Entity::takeNewEntityPacket(ECCPacket& packet) { return true; }
#else
void Entity::makeNewEntityPacket(ECCPacket& packet) const
{
    packet.clear();

    packet << Networking::StoC::EntityAction;
    packet << EntityActions::StoC::AddEntity;
    packet << getEntityCode();
    packet << getId();

    addInfoToNewEntityPacket(packet);
}

void Entity::addInfoToNewEntityPacket(ECCPacket& packet) const {}
#endif // CLIENT_SIDE
