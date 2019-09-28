#pragma once

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../../common-source/Entities/Entity.h"

class EntitiesManager
{
    public:
        EntitiesManager(World* world);
        ~EntitiesManager();

        /**
         * Update all entities
         * @param delta : the delta time (time since last update)
         */
        void updateAll(float delta);
        /**
         * Draw all entities
         * @param target : the render target on which to draw on
         */
        void drawAll(sf::RenderTarget& target) const;

        /**
         * Adds an entity
         * If the entity couldnt be added, it will be deleted
         * @param entity : the pointer to the entity to add
         * @return True if the entity was added, False if it wasn't and deleted
         */
        //bool addEntity(Entity* entity);

        /**
         * Reads an entity-related packet
         * This function is thread safe
         * @param packet : the packet to read
         * @return True if the packet was successfully read and something was done
         */
        bool readEntityPacket(sf::Packet& packet);

        //Just for when we add entities from an other thread
        mutable sf::Mutex entities_mutex;

    private:
        World* const world;

        //We have two containers
        std::map<unsigned int, Entity*> entities_map;   //One to get an entity by its id
        mutable std::vector<Entity*> entities_vector;   //One to easily iterate through entities, that we keep sorted for rendering

        //Used for sorting entities
        inline static bool compareY(Entity* a, Entity* b)
        {
            return a->getPosition().y < b->getPosition().y;
        }

        /**
         * Adds an entity (in case of AddEntity code in readEntityPacket)
         * This function is thread safe
         * @param packet : the packet to read
         * @return True if the entity was added
         */
        bool addEntity(sf::Packet& packet);
};
