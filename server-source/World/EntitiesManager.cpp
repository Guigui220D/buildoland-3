#include "EntitiesManager.h"

#include "../Server/Server.h"

#include <SFML/Network.hpp>
#include "../../common-source/Networking/NetworkingCodes.h"
#include "../../common-source/Entities/EntityCodes.h"

EntitiesManager::EntitiesManager(Server& server) :
    server(server),
    next_entity_id(0)
{
    //ctor
}

EntitiesManager::~EntitiesManager()
{
    for (auto i = entities.begin(); i != entities.end(); i++)
        delete i->second;
}

void EntitiesManager::updateAll(float delta)
{
    entities_mutex.lock();
    for (auto i = entities.begin(); i != entities.end(); i++)
        i->second->updateBase(delta);
    entities_mutex.unlock();
}

bool EntitiesManager::newEntity(Entity* entity)
{
    sf::Lock lock(entities_mutex);

    if (entities.find(entity->getId()) != entities.cend())
    {
        delete entity;
        return false;
    }

    entities.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));

    sf::Packet packet;
    entity->makeNewEntityPacket(packet);

    server.getClientsManager().sendToAll(packet);

    return true;
}

void EntitiesManager::removeEntity(unsigned int id)
{
    sf::Lock lock(entities_mutex);

    if (entities.find(id) != entities.cend())
        entities.erase(entities.find(id));

    sf::Packet packet;
    packet << Networking::StoC::EntityAction;
    packet << EntityActions::StoC::ForgetEntity;

    packet << id;

    server.getClientsManager().sendToAll(packet);
}

void EntitiesManager::sendAddEntityFromAllEntitiesInChunk(sf::Vector2i chunk_pos, const Client& client)
{
    sf::Lock lock(entities_mutex);

    sf::Packet packet;
    for (auto i = entities.begin(); i != entities.end(); i++)
    {
        Entity* entity = i->second;
        if (entity->getChunkOn() == chunk_pos)
        {
            entity->makeNewEntityPacket(packet);
            client.send(packet);
        }
    }
}

void EntitiesManager::sendAddEntityToClient(unsigned int id, const Client& client)
{
    sf::Lock lock(entities_mutex);

    auto i = entities.find(id);
    if (i == entities.end())
        return;

    sf::Packet packet;
    i->second->makeNewEntityPacket(packet);
    client.send(packet);
}
