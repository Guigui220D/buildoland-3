#pragma once

#include "EntityCodes.h"

#include <cmath>

#ifdef CLIENT_SIDE
    #include <SFML/Graphics.hpp>
    #include <SFML/Network.hpp>
#else
    #include <SFML/System.hpp>
#endif // CLIENT_SIDE

class World;

class Entity
{
    public:
        virtual inline unsigned short getEntityCode() const = 0;

        Entity(World* world, unsigned int id);
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
         * Receives an entity action packet
         * @param packet : the packet to read
         * @return true if the packet was succesfully read and something was done
         */
        virtual bool takePacket(sf::Packet& packet) = 0;
        #endif // CLIENT_SIDE

        inline unsigned int getId() const { return id; }

        inline sf::Vector2f getPosition() const { return position; }
        inline World* getWorld() const { return world; }

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

    private:
        void onChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk);

        const unsigned int id;
        World* const world;

        sf::Vector2i chunk_on;
};
