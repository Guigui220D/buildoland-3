#include "EntitiesManager.h"

#include <algorithm>
#include <cassert>

#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/StoC_EntityActionCodes.h"

#include "../../common-source/Entities/EntityCodes.h"

#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Entities/GameEntities/TestEntity.h"
#include "../../common-source/Entities/GameEntities/DroppedItemEntity.h"

#include "../../client-source/World/World.h"

#include "../../common-source/Utils/Log.h"

#include "../States/GameState.h"

#include "../Packets/RequestEntityInfoPacket.h"

EntitiesManager::EntitiesManager(World& world) :
    world(world)
{
    //ctor
}

EntitiesManager::~EntitiesManager()
{
    for (Entity*& entity : entities_vector)
        delete entity;
}

void EntitiesManager::updateAll(float delta)
{
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
}

void EntitiesManager::sortDrawOrder()
{
    std::sort(entities_vector.begin(), entities_vector.end(), EntitiesManager::drawSortCompare);
}

void EntitiesManager::drawAll(sf::RenderTarget& target) const
{
    for (Entity*& entity : entities_vector)
        entity->draw(target);
}

void EntitiesManager::drawAllBelow(sf::RenderTarget& target) const
{
    for (Entity*& entity : entities_vector)
        entity->drawBelow(target);
}

void EntitiesManager::drawAllAbove(sf::RenderTarget& target) const
{
    for (Entity*& entity : entities_vector)
        entity->drawAbove(target);
}

Entity* EntitiesManager::getEntity(unsigned int id)
{
    auto i = entities_map.find(id);
	if (i == entities_map.end() || i->second->to_be_removed)
		return nullptr;
	return i->second;
}

bool EntitiesManager::readEntityPacket(ECCPacket& packet)
{
    int entity_action_code; packet >> entity_action_code;

    if (!packet)
    {
        log(ERROR, "Entity packet was too short (reading entity action code).\n");
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
        log(ERROR, "Entity action code unknown.\n");
        return false;
    }
}

bool EntitiesManager::drawSortCompare(Entity *a, Entity *b)
{
    if (a->getZOrder() == b->getZOrder())
    {
        return (a->getPosition().y == b->getPosition().y) ? a->getPosition().x < b->getPosition().x
                                                          : a->getPosition().y < b->getPosition().y;
    }
    else
        return a->getZOrder() < b->getZOrder();
}

bool EntitiesManager::addEntity(ECCPacket& packet)
{
    unsigned short entity_code; packet >> entity_code;
    unsigned int entity_id; packet >> entity_id;

    if (!packet)
    {
        log(ERROR, "Entity packet was too short (reading type code and id).\n");
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
        case Entities::None: return true;

        case Entities::Player: new_entity = new Player(world, entity_id); break;

        case Entities::TestEntity: new_entity = new TestEntity(world, entity_id); break;

        case Entities::DroppedItemEntity: new_entity = new DroppedItemEntity(world, entity_id); break;

        default: log(ERROR, "Entity type code unknown.\n"); return false;
    }

    assert(new_entity);

    new_entity->takeNewEntityPacket(packet);

    auto ent_i = entities_map.find(entity_id);

    if (ent_i != entities_map.cend())
    {
        //log(ERROR, "Another entity with the same ID exists, deleting it.\n");

        entities_vector.erase(std::remove(entities_vector.begin(), entities_vector.end(), ent_i->second), entities_vector.end());
        delete ent_i->second;
        entities_map.erase(ent_i);
    }

    entities_map.emplace(std::pair<unsigned int, Entity*>(entity_id, new_entity));
    entities_vector.push_back(new_entity);

    return true;
}

void EntitiesManager::removeEntity(ECCPacket& packet)
{
    unsigned int entity_id;
    if (!(packet >> entity_id))
    {
        log(ERROR, "Entity packet was too short (reading entity id).\n");
        return;
    }

    if (entity_id == Player::this_player_id)
        return;

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

bool EntitiesManager::doEntityAction(ECCPacket& packet)
{
    unsigned int id;
    if (!(packet >> id))
    {
        log(ERROR, "Entity packet was too short (reading entity action code).\n");
        return false;
    }
    if (id == Player::this_player_id)
    {
        //log(INFO, "YEAH BOIII IT'S ME!\n");
        return true;
    }

    Entity* en = getEntity(id);

    if (!en)
    {
        log(ERROR, "The entity with that code couldn't be found (doEntityAction). Sending a request.\n");

        //We ask the server to tell us about that entity because we don't have it
        RequestEntityInfoPacket request(id);
        world.getState().sendToServer(request);
        return false;
    }

    return en->takePacket(packet);
}
