#pragma once

#include <unordered_map>
#include <vector>

#include "../../common-source/Networking/ECCPacket.h"

class Entity;
class World;
class Chunk;

namespace sf { class RenderTarget; }

class EntitiesManager
{
    friend class World;
    friend class GameState;

    public:
        EntitiesManager(World& world);
        ~EntitiesManager();

        /**
         * Update all entities
         * @param delta : the delta time (time since last update)
         */
        void updateAll(float delta);
        /**
         * Sort entities for rendering
         */
        void sortDrawOrder();
        /**
         * Draw all entities
         * @param target : the render target on which to draw on
         */
        void drawAll(sf::RenderTarget& target) const;

        /**
         * Draw parts of all entities that are on a lower z layer
         * @param target : the render target on which to draw on
         */
        void drawAllBelow(sf::RenderTarget& target) const;

        /**
         * Draw parts of all entities that are above everything
         * @param target : the render target on which to draw on
         */
        void drawAllAbove(sf::RenderTarget& target) const;

        /**
         * Get a pointer to an entity using its id
         * @param id : the id of the entity you're looking for
         * @return the pointer to the entity, or nullptr if it doesn't exist
         */
        Entity* getEntity(unsigned int id);

        /**
         * Reads an entity-related packet
         * This function is thread safe
         * @param packet : the packet to read
         * @return True if the packet was successfully read and something was done
         */
        bool readEntityPacket(ECCPacket& packet);

    private:
        World& world;

        bool gotPlayer = false;

        //We have two containers
        std::unordered_map<unsigned int, Entity*> entities_map;   //One to get an entity by its id
        mutable std::vector<Entity*> entities_vector;   //One to easily iterate through entities, that we keep sorted for rendering

        //Used for sorting entities
        static bool drawSortCompare(Entity* a, Entity* b);

        /**
         * Adds an entity (in case of AddEntity code in readEntityPacket)
         * This function is thread safe
         * @param packet : the packet to read
         * @return True if the entity was added
         */
        bool addEntity(ECCPacket& packet);
        /**
         * Removes an entity
         * This function is thread safe
         * @param packet : the packet to read
         */
        void removeEntity(ECCPacket& packet);
        /**
         * Sends an action packet to the right entity
         * This function is thread safe
         * @param packet : the packet to read
         * @return true if the entity was found and something was done
         */
        bool doEntityAction(ECCPacket& packet);
};
