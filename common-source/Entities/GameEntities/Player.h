#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Utils/Animation.h"
#else
    class Client;
#endif // CLIENT_SIDE

class Chunk;

class Player : public LivingEntity
{
    public:
        #ifdef CLIENT_SIDE
        static unsigned int this_player_id;
        #endif // CLIENT_SIDE

        inline unsigned short getEntityCode() const { return Entities::Player; };

        #ifdef CLIENT_SIDE
            Player(World* world, unsigned int id);
        #else
            Player(World* world, unsigned int id, const Client& client);
        #endif // CLIENT_SIDE
        ~Player();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;
        #endif

        bool isSubscribedTo(sf::Vector2i chunk) const;
    private:
        #ifdef CLIENT_SIDE
        void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) override;

        sf::RectangleShape rs;
        sf::CircleShape shadow;
        #else
        const Client& client;
        #endif
};
