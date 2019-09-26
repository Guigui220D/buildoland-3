#pragma once

#include "PhysicsEntity.h"

#ifdef CLIENT_SIDE
    #include "../Utils/Animation.h"
#endif // CLIENT_SIDE

class LivingEntity : public PhysicsEntity
{
    public:
        LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed);
        virtual ~LivingEntity();

    protected:
        float walking_speed;
        sf::Vector2f walking_direction;

        void walk(float delta);

        #ifdef CLIENT_SIDE
        inline sf::IntRect getCurrentTextureRect() const { return animation.getCurrentTextureRect(); }

    private:
        Animation<64, 5, 1> animation;
        #endif
};
