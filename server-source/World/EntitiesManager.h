#pragma once

#include <map>

#include "../../common/Entities/Entity.h"

class EntitiesManager
{
    public:
        EntitiesManager();
        ~EntitiesManager();

        /**
         * Update all entities
         * @param delta : the delta time (time since last update)
         */
        void updateAll(float delta);

        /**
         * Adds an entity
         * If the entity couldnt be added, it will be deleted
         * @param entity : the pointer to the entity to add
         * @return True if the entity was added, False if it wasn't and deleted
         */
        bool addEntity(Entity* entity);

    private:
        std::map<unsigned int, Entity*> entities;
};
