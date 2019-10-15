#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Utils/Animation.h"
#endif // CLIENT_SIDE

class Chunk;

class Player : public LivingEntity
{
    public:
        inline unsigned short getEntityCode() const { return Entities::Player; };

        Player(World* world, unsigned int id);
        ~Player();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;
        #endif

        bool isSubscribedTo(const Chunk* chunk) const;
    private:
        #ifdef CLIENT_SIDE
        sf::RectangleShape rs;
        sf::CircleShape shadow;
        #endif
};
