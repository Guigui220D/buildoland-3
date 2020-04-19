#include "EntitiesManager.h"

#include <algorithm>
#include <cassert>

#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/StoC_EntityActionCodes.h"

#include "../../common-source/Entities/EntityCodes.h"
#include "../../common-source/Entities/TileEntityCodes.h"

#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Entities/GameEntities/TestEntity.h"
#include "../../common-source/Entities/GameEntities/DroppedItemEntity.h"

#include "../../common-source/Entities/GameTileEntities/TestTileEntity.h"
#include "../../common-source/Entities/GameTileEntities/TreeTopEntity.h"

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

void EntitiesManager::drawAll(sf::RenderTarget& target) const
{
    //Sorting the entities by y position
    std::sort(entities_vector.begin(), entities_vector.end(), EntitiesManager::compareY);

    for (Entity*& entity : entities_vector)
        entity->draw(target);
}

void EntitiesManager::drawAllAbove(sf::RenderTarget& target) const
{
    std::sort(entities_vector.begin(), entities_vector.end(), EntitiesManager::compareY);

    for (Entity*& entity : entities_vector)
        entity->drawAbove(target);
}

Entity* EntitiesManager::getEntity(unsigned int id)
{
    auto i = entities_map.find(id);
    return (i != entities_map.end() ? i->second : nullptr);
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

bool EntitiesManager::compareY(Entity *a, Entity *b)
{
    return (a->getPosition().y == b->getPosition().y) ? a->getPosition().x < b->getPosition().x
                                                      : a->getPosition().y < b->getPosition().y;
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

    case Entities::TileEntity:
        {
            unsigned short tile_entity_code; packet >> tile_entity_code;

            sf::Vector2i tile_pos;
            packet >> tile_pos.x >> tile_pos.y;

            if (!packet)
            {
                log(ERROR, "Tile entity packet was too short.\n");
                return false;
            }

            switch (tile_entity_code)
            {
            case TileEntities::None: return true;

            case TileEntities::TestTileEntity: new_entity = new TestTileEntity(world, entity_id, tile_pos); break;

            case TileEntities::TreeTopEntity: new_entity = new TreeTopEntity(world, entity_id, tile_pos); break;

            default: log(ERROR, "Tile entity type code unknown.\n"); return false;
            }
        }
        break;

    case Entities::Player: new_entity = new Player(world, entity_id); break;

    case Entities::TestEntity: new_entity = new TestEntity(world, entity_id); break;

    case Entities::DroppedItemEntity: new_entity = new DroppedItemEntity(world, entity_id);
    break;

    default: log(ERROR, "Entity type code unknown.\n"); return false;
    }

    assert(new_entity);

    if (new_entity->isTileEntity())
        if (world.isChunkLoaded(new_entity->getChunkOn()))
        {
            TileEntity* te = (TileEntity*) new_entity;
            te->assignChunk(&world.getChunk(new_entity->getChunkOn()));
        }

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

void EntitiesManager::declareNewChunkForTileEntities(Chunk* new_chunk)
{
    for (Entity*& entity : entities_vector)
    {
        if (entity->isTileEntity())
        {
            TileEntity* te = (TileEntity*) entity;

            if (!te->isReady())
                te->assignChunk(new_chunk);
        }
    }
}
