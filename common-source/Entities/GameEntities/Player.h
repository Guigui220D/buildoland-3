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
        inline unsigned short getEntityCode() const { return Entities::Player; };

        #ifdef CLIENT_SIDE
            Player(World* world, unsigned int id);
        #else
            Player(World* world, unsigned int id, Client& client);
        #endif // CLIENT_SIDE
        ~Player();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;
        #endif

        bool isSubscribedTo(sf::Vector2i chunk) const;
    private:
        #ifdef CLIENT_SIDE
        sf::RectangleShape rs;
        sf::CircleShape shadow;
        #else
        Client& client;
        #endif
};
