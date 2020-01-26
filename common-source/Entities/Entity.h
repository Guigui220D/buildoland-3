#pragma once

#include "EntityCodes.h"

#include <cmath>
#include <SFML/Network.hpp>

#ifdef CLIENT_SIDE
    #include <SFML/Graphics.hpp>
#else
    #include "../Networking/NetworkingCodes.h"
    #include <SFML/System.hpp>
#endif // CLIENT_SIDE

class World;

class Entity
{
    public:
        virtual inline unsigned short getEntityCode() const { return Entities::None; };

        Entity(World& world, unsigned int id);
        virtual ~Entity();

        /**
         * Updates this entity
         * Calls update()
         * @param delta : the elapsed time
         */
        void updateBase(float delta);

        #ifdef CLIENT_SIDE
        /**
         * Draw this entity
         * @param target : the render target on which to draw
         */
        virtual void draw(sf::RenderTarget& target) const;

        /**
         * Reads an entity action packet
         * @param packet : the packet to read
         * @return true if the packet was succesfully read and something was done
         */
        virtual bool takePacket(sf::Packet& packet);
        /**
         * Reads a new entity packet
         * @param packet : the packet to read
         * @return true if the packet was succesfully read and something was done
         */
        virtual bool takeNewEntityPacket(sf::Packet& packet);

        bool to_be_removed = false;
        #else
        /**
         * Makes a new entity packet
         * The packet is composed, then addInfoToNewEntityPacket is called on it to add custom info depending on the packet
         * @param packet : the packet to make in
         */
        virtual void makeNewEntityPacket(sf::Packet& packet) const;
        #endif // CLIENT_SIDE

        inline unsigned int getId() const { return id; }

        inline sf::Vector2f getPosition() const { return position; }
        inline World& getWorld() const { return world; }

        inline sf::Vector2i getBlockOn() const { return sf::Vector2i(std::floor(position.x + .5f), std::floor(position.y + .5f)); }
        sf::Vector2i getChunkOn() const;

    protected:
        /**
         * Updates this entity
         * This function is meant to be overriden
         * @param delta : time since last update
         */
        virtual void update(float delta);

        sf::Vector2f position;

        #ifndef CLIENT_SIDE
        /**
         * Sends a packet to all clients who know of this entity
         * @param packet : the packet to send
         */
        void send(sf::Packet& packet);
        /**
         * Adds custom info to the packet that will be sent to new entity
         * @param packet : the packet append to
         */
        virtual void addInfoToNewEntityPacket(sf::Packet& packet) const;
        #endif // CLIENT_SIDE

        sf::Vector2i chunk_on;

    private:
        void onChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk);
        #ifdef CLIENT_SIDE
        virtual void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk);
        #endif // CLIENT_SIDE

        const unsigned int id;
        World& world;
};
