#include "EntitiesManager.h"

#include <algorithm>

EntitiesManager::EntitiesManager()
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

bool EntitiesManager::addEntity(Entity* entity)
{
    if (entities.find(entity->getId()) != entities.cend())
    {
        delete entity;
        return false;
    }

    entities.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));

    return true;
}
