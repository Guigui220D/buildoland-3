#include "EntitiesManager.h"

#include <algorithm>
#include <iostream>
#include <assert.h>

#include <SFML/Network.hpp>

#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/StoC_EntityActionCodes.h"
#include "../../common-source/Entities/EntityCodes.h"

#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Entities/GameEntities/TestEntity.h"

#include "../States/GameState.h"

EntitiesManager::EntitiesManager(World& world) :
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
    for (auto i = entities_vector.begin(); i != entities_vector.end();)
    {
        if ((*i)->to_be_removed)
        {
            //Removes the entity from the containers
            assert(entities_map.find((*i)->getId()) != entities_map.end());

            entities_map.erase(entities_map.find((*i)->getId()));
            delete (*i);
            i = entities_vector.erase(i);
        }
        else
            i++;
    }

    for (Entity*& entity : entities_vector)
        entity->updateBase(delta);
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

Entity* EntitiesManager::getEntity(unsigned int id)
{
    sf::Lock lock(entities_mutex);
    auto i = entities_map.find(id);
    return (i != entities_map.end() ? i->second : nullptr);
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

    case EntityActions::StoC::ForgetEntity:
        removeEntity(packet);
        return true;

    case EntityActions::StoC::EntityAction:
        return doEntityAction(packet);

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

    if (entity_id == Player::this_player_id)
    {
        if (gotPlayer)
            return true;

        gotPlayer = true;
    }


    Entity* new_entity = nullptr;

    switch (entity_code)
    {
    case Entities::Player:
        new_entity = new Player(world, entity_id);
        break;
    case Entities::TestEntity:
        new_entity = new TestEntity(world, entity_id);
        break;
    default:
        std::cerr << "Entity type code unknown." << std::endl;
        return false;
    }

    assert(new_entity);

    new_entity->takeNewEntityPacket(packet);

    sf::Lock lock(entities_mutex);

    auto ent_i = entities_map.find(entity_id);

    if (ent_i != entities_map.cend())
    {
        std::cerr << "Another entity with the same ID exists, deleting it." << std::endl;

        entities_vector.erase(std::remove(entities_vector.begin(), entities_vector.end(), ent_i->second), entities_vector.end());
        delete ent_i->second;
        entities_map.erase(ent_i);
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

    if (entity_id == Player::this_player_id)
        return;

    sf::Lock lock(entities_mutex);

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

bool EntitiesManager::doEntityAction(sf::Packet& packet)
{
    unsigned int id;
    if (!(packet >> id))
    {
        std::cerr << "Entity packet was too short (reading entity action code)." << std::endl;
        return false;
    }
    if (id == Player::this_player_id)
    {
        //std::cout << "YEAH BOIII IT'S ME!" << std::endl;
        return true;
    }

    Entity* en = getEntity(id);

    if (!en)
    {
        std::cerr << "The entity with that code couldn't be found (doEntityAction). Sending a request." << std::endl;

        //We ask the server to tell us about that entity because we don't have it
        sf::Packet request;
        request << Networking::CtoS::RequestEntityInfo;
        request << id;
        world.getState().sendToServer(request);
        return false;
    }

    sf::Lock lock(entities_mutex);

    return en->takePacket(packet);
}
