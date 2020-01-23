#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Utils/Animation.h"
#else
    class Client;
#endif // CLIENT_SIDE

#include "../../Items/playerInventory.h"

class Chunk;

class Player : public LivingEntity
{
    public:
        #ifdef CLIENT_SIDE
        static unsigned int this_player_id;
        static Player* this_player;
        #endif // CLIENT_SIDE

        inline unsigned short getEntityCode() const { return Entities::Player; };

        #ifdef CLIENT_SIDE
            Player(World& world, unsigned int id);
        #else
            Player(World& world, unsigned int id, const Client& client);
        #endif // CLIENT_SIDE
        ~Player();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;

        void useHand(sf::Vector2i pos);

        bool takeNewEntityPacket(sf::Packet& packet) override;
        #else
        void takePlayerActionPacket(sf::Packet& packet);

        inline const Client& getClient() const { return client; }

        void makeNewEntityPacket(sf::Packet& packet) const override;
        #endif

        inline void setPosition(sf::Vector2f new_pos) { position = new_pos; }

        bool isSubscribedTo(sf::Vector2i chunk) const;

        inline PlayerInventory& getInventory() { return inventory; }

    private:
        PlayerInventory inventory;

        std::array<uint32_t, 4> outfit_colors;

        #ifdef CLIENT_SIDE
        void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) override;

        sf::RectangleShape base;
        sf::RectangleShape shoes;
        sf::RectangleShape pants;
        sf::RectangleShape shirt;
        sf::CircleShape shadow;

        sf::Vector2f last_walking_direction;
        sf::Clock frequent_walk_update;
        #else
        const Client& client;
        #endif
};
