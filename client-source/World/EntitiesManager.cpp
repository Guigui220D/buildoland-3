#include "EntitiesManager.h"

#include <algorithm>
#include <iostream>
#include <assert.h>

#include <SFML/Network.hpp>

#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/StoC_EntityActionCodes.h"
#include "../../common-source/Entities/EntityCodes.h"

#include "../../common-source/Entities/GameEntities/TestEntity.h"

EntitiesManager::EntitiesManager(World* world) :
    world(world)
{
    //ctor
}

EntitiesManager::~EntitiesManager()
{
    entities_mutex.lock();
    for (Entity*& entity : entities_vector)
        delete entity;
    entities_mutex.unlock();
}

void EntitiesManager::updateAll(float delta)
{
    entities_mutex.lock();
    for (Entity*& entity : entities_vector)
        entity->update(delta);
    entities_mutex.unlock();
}

void EntitiesManager::drawAll(sf::RenderTarget& target) const
{
    entities_mutex.lock();
    //Sorting the entities by y position
    std::sort(entities_vector.begin(), entities_vector.end(), EntitiesManager::compareY);

    for (Entity*& entity : entities_vector)
        entity->draw(target);
    entities_mutex.unlock();
}

bool EntitiesManager::readEntityPacket(sf::Packet& packet)
{
    unsigned short entity_action_code;
    if (!(packet >> entity_action_code))
    {
        std::cerr << "Entity packet was too short (reading entity action code)." << std::endl;
        return false;
    }

    switch (entity_action_code)
    {
    case EntityActions::StoC::AddEntity:
        return addEntity(packet);

    case EntityActions::StoC::RemoveEntity:
        removeEntity(packet);
        return true;

    case EntityActions::StoC::EntityAction:
        std::cerr << "EntityAction code unimplemented." << std::endl;
        return false;
    default:
        std::cerr << "Entity action code unknown." << std::endl;
        return false;
    }
}

bool EntitiesManager::addEntity(sf::Packet& packet)
{
    unsigned short entity_code;
    if (!(packet >> entity_code))
    {
        std::cerr << "Entity packet was too short (reading type code)." << std::endl;
        return false;
    }

    unsigned int entity_id;
    if (!(packet >> entity_id))
    {
        std::cerr << "Entity packet was too short (reading entity id)." << std::endl;
        return false;
    }

    Entity* new_entity = nullptr;

    switch (entity_code)
    {
    case Entities::Player:
        std::cerr << "Player unimplemented." << std::endl;
        return false;
    case Entities::TestEntity:
        new_entity = new TestEntity(world, entity_id);
        break;
    default:
        std::cerr << "Entity type code unknown." << std::endl;
        return false;
    }

    assert(new_entity);

    sf::Lock lock(entities_mutex);

    if (entities_map.find(entity_id) != entities_map.cend())
    {
        std::cerr << "Could not add entity, another one with the same ID exists, deleting it." << std::endl;
        delete new_entity;
        return false;
    }

    entities_map.emplace(std::pair<unsigned int, Entity*>(entity_id, new_entity));
    entities_vector.push_back(new_entity);

    return true;
}

void EntitiesManager::removeEntity(sf::Packet& packet)
{
    unsigned int entity_id;
    if (!(packet >> entity_id))
    {
        std::cerr << "Entity packet was too short (reading entity id)." << std::endl;
        return;
    }

    auto map_i = entities_map.find(entity_id);
    if (map_i == entities_map.end())
        return;

    Entity* entity_ptr = map_i->second;
    entities_map.erase(map_i);

    for (auto vector_i = entities_vector.begin(); vector_i != entities_vector.end(); vector_i++)
    {
        if (*vector_i == entity_ptr)
        {
            entities_vector.erase(vector_i);
            break;
        }
    }

    delete entity_ptr;
}

/*
bool EntitiesManager::addEntity(Entity* entity)
{
    sf::Lock lock(entities_mutex);

    if (entities_map.find(entity->getId()) != entities_map.cend())
    {
        delete entity;
        return false;
    }

    entities_map.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));
    entities_vector.push_back(entity);

    return true;
}
*/
