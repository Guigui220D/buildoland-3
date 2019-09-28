#pragma once

#include <map>

#include "../../common-source/Entities/Entity.h"

class Server;

class EntitiesManager
{
    public:
        EntitiesManager(Server* server);
        ~EntitiesManager();

        /**
         * Update all entities
         * @param delta : the delta time (time since last update)
         */
        void updateAll(float delta);

        /**
         * Adds a new entity
         * If the entity couldnt be added, it will be deleted
         * @param entity : the pointer to the entity to add
         * @return True if the entity was added, False if it wasn't and deleted
         */
        bool newEntity(Entity* entity);

    private:
        std::map<unsigned int, Entity*> entities;

        Server* const server;
};
