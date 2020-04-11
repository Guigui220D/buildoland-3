#pragma once

#include <unordered_map>

#include <SFML/System/Vector2.hpp>

class Client;
class Entity;
class Server;

class EntitiesManager
{
    public:
        EntitiesManager(Server& server);
        ~EntitiesManager();

        /**
         * Update all entities
         * @param delta : the delta time (time since last update)
         */
        void updateAll(float delta);

        /**
         * Adds a new entity
         * If the entity couldnt be added, it will be deleted
         * A packet will be sent to all the clients to inform them
         * @param entity : the pointer to the entity to add
         * @param declare : if the new entity should be sent to all players in the chunk (false if that entity was generated WITH the chunk)
         * @return True if the entity was added, False if it wasn't and deleted
         */
        bool newEntity(Entity* entity, bool declare = true);

        /**
         * Removes an entity using its id
         * A packet will be sent to all the clients to inform them
         * @param id : the id of the entity to remove
         */
        void removeEntity(unsigned int id);

        /**
         * Sends a addEntity from all entities in chunk to the client
         * @param chunk_pos : the chunk
         * @param client : the client to send to
         */
        void sendAddEntityFromAllEntitiesInChunk(sf::Vector2i chunk_pos, const Client& client);
        /**
         * Sends a addEntity of one entity to a client
         * @param id : the id of the entity to send info of to the client
         * @param client : the client to send to
         */
        void sendAddEntityToClient(unsigned int id, const Client& client);

        inline unsigned int getNextEntityId() { return next_entity_id++; }

    private:
        std::unordered_map<unsigned int, Entity*> entities;

        Server& server;

        unsigned int next_entity_id;
};
