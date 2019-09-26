#include "EntitiesManager.h"

#include <algorithm>

EntitiesManager::EntitiesManager()
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
    for (Entity*& entity : entities_vector)
        entity->update(delta);
}

void EntitiesManager::drawAll(sf::RenderTarget& target) const
{
    //Sorting the entities by y position
    std::sort(entities_vector.begin(), entities_vector.end(), EntitiesManager::compareY);

    for (Entity*& entity : entities_vector)
        entity->draw(target);
}

bool EntitiesManager::addEntity(Entity* entity)
{
    if (entities_map.find(entity->getId()) != entities_map.cend())
    {
        delete entity;
        return false;
    }

    entities_map.emplace(std::pair<unsigned int, Entity*>(entity->getId(), entity));
    entities_vector.push_back(entity);

    return true;
}
