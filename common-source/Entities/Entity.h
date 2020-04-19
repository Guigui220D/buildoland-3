#pragma once

#include "EntityCodes.h"

#include <cmath>
#include "../Networking/ECCPacket.h"

#include <SFML/System/Vector2.hpp>

#ifdef CLIENT_SIDE
namespace sf
{
class RenderTarget;
}
#else
    #include "../Networking/NetworkingCodes.h"
#endif // CLIENT_SIDE

class World;
class Player;

class Entity
{
    friend class GameState;

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

        /**
         * Called when the client makes a right click on the entity / when the server receives a right click request
         */
        virtual void onLeftClick(Player& player)
        {}
        /**
         * Called when the client makes a left click on the entity / when the server receives a left click requesty
         */
        virtual void onRightClick(Player& player)
        {}

        #ifdef CLIENT_SIDE
        /**
         * Draw this entity
         * @param target : the render target on which to draw
         */
        virtual void draw(sf::RenderTarget& target) const;
        /**
         * Draws more things that should be drawn over block tops
         * @param target : the render target on which to draw
         */
        virtual void drawAbove(sf::RenderTarget& target) const;

        /**
         * Reads an entity action packet
         * @param packet : the packet to read
         * @return true if the packet was succesfully read and something was done
         */
        virtual bool takePacket(ECCPacket& packet);
        /**
         * Reads a new entity packet
         * @param packet : the packet to read
         * @return true if the packet was succesfully read and something was done
         */
        virtual bool takeNewEntityPacket(ECCPacket& packet);
        /**
         * Gets the physics bounding box of the entity
         * @return the physcis bounding box of the entity
         */
        virtual sf::Vector2f getHitbox() const = 0;
        /**
         * Gets the visual bounding box of the entity
         * @return the visual bounding box of the entity
         */
        virtual sf::Vector2f getVisualHitbox() const
        { return getHitbox(); }
        #else
        /**
         * Makes a new entity packet
         * The packet is composed, then addInfoToNewEntityPacket is called on it to add custom info depending on the packet
         * @param packet : the packet to make in
         */
        virtual void makeNewEntityPacket(ECCPacket& packet) const;
        #endif // CLIENT_SIDE

        bool to_be_removed = false;

        inline unsigned int getId() const { return id; }

        inline bool isHighlighted() const
        { return highlighted; }

        inline sf::Vector2f getPosition() const { return position; }
        inline World& getWorld() const { return world; }

        inline sf::Vector2i getBlockOn() const { return sf::Vector2i(std::floor(position.x + .5f), std::floor(position.y + .5f)); }
        sf::Vector2i getChunkOn() const;

        virtual inline bool isTileEntity() const { return false; }

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
        void send(ECCPacket& packet);
        /**
         * Adds custom info to the packet that will be sent to new entity
         * @param packet : the packet append to
         */
        virtual void addInfoToNewEntityPacket(ECCPacket& packet) const;
        #endif // CLIENT_SIDE

        sf::Vector2i chunk_on;

    private:
        void onChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk);
        #ifdef CLIENT_SIDE
        virtual void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk);
        #endif // CLIENT_SIDE

        const unsigned int id;
        World& world;
        bool highlighted = false;
};
