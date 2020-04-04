#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../../common-source/Entities/Entity.h"
#include "../../common-source/Entities/TileEntity.h"

class EntitiesManager
{
    friend class World;

    public:
        EntitiesManager(World& world);
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
         * Draw parts of all entities taht are above everything
         * @param target : the render target on which to draw on
         */
        void drawAllAbove(sf::RenderTarget& target) const;

        void declareNewChunkForTileEntities(Chunk* new_chunk);

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
        inline static bool compareY(Entity* a, Entity* b)
        {
            return (a->getPosition().y == b->getPosition().y) ? a->getPosition().x < b->getPosition().x
                                                              : a->getPosition().y < b->getPosition().y;
        }

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
