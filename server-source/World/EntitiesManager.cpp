#include "EntitiesManager.h"

#include "../Server/Server.h"

#include "../../common-source/Entities/Entity.h"
#include "../../common-source/Networking/NetworkingCodes.h"
#include "../../common-source/Entities/EntityCodes.h"
#include "../Packets/ForgetEntityPacket.h"

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
    for (auto i = entities.begin(); i != entities.end();)
    {
        if (i->second->to_be_removed)
        {
            ForgetEntityPacket packet(i->second->getId());
            server.getClientsManager().sendToAll(packet);

            delete i->second;
            i = entities.erase(i);
        }
        else
            i++;
    }

    for (auto i = entities.begin(); i != entities.end(); i++)
        i->second->updateBase(delta);
}

bool EntitiesManager::newEntity(Entity* entity, bool declare)
{
    if (entities.find(entity->getId()) != entities.cend())
    {
        delete entity;
        return false;
    }

    entities.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));

    if (declare)
    {
        ECCPacket packet;
        entity->makeNewEntityPacket(packet);

        server.getClientsManager().sendToAll(packet);
    }

    return true;
}

void EntitiesManager::removeEntity(unsigned int id)
{
    if (entities.find(id) != entities.cend())
        entities.erase(entities.find(id));

    ForgetEntityPacket packet(id);
    server.getClientsManager().sendToAll(packet);
}

Entity *EntitiesManager::getEntity(unsigned int id)
{
    if (entities.find(id) != entities.cend() && !entities.find(id)->second->to_be_removed)
        return entities.find(id)->second;
    else
        return nullptr;
}

void EntitiesManager::sendAddEntityFromAllEntitiesInChunk(sf::Vector2i chunk_pos, const Client& client)
{
    ECCPacket packet;
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
    auto i = entities.find(id);
    if (i == entities.end())
        return;

    ECCPacket packet;
    i->second->makeNewEntityPacket(packet);
    client.send(packet);
}

void EntitiesManager::popEntitiesOfChunk(sf::Vector2i chunk_pos, std::vector<Entity*>& vec)
{
    for (auto i = entities.begin(); i != entities.end(); )
    {
        Entity* entity = i->second;

        if (entity->getChunkOn() == chunk_pos)
        {
            i = entities.erase(i);
            vec.emplace_back(entity);
        }
        else
            i++;
    }
}
