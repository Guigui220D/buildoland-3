#pragma once

#include "EntityCodes.h"

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
         * @param delta : time since last update
         */
        virtual void update(float delta);

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

    protected:
        sf::Vector2f position;

    private:
        const unsigned int id;
        World* const world;
};
