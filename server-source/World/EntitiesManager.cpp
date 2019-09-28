#include "EntitiesManager.h"

#include "../Server/Server.h"

#include <SFML/Network.hpp>
#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/StoC_EntityActionCodes.h"
#include "../../common-source/Entities/EntityCodes.h"

EntitiesManager::EntitiesManager(Server* server) :
    server(server)
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
    for (auto i = entities.begin(); i != entities.end(); i++)
        i->second->update(delta);
}

bool EntitiesManager::newEntity(Entity* entity)
{
    if (entities.find(entity->getId()) != entities.cend())
    {
        delete entity;
        return false;
    }

    entities.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));

    sf::Packet packet;
    packet << (unsigned short)Networking::StoC::EntityAction;
    packet << (unsigned short)EntityActions::StoC::AddEntity;
    packet << (unsigned short)entity->getEntityCode();

    server->getClientsManager().sendToAll(packet);

    return true;
}