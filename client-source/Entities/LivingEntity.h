#pragma once

#include "PhysicsEntity.h"
#include "../Utils/Animation.h"

class LivingEntity : public PhysicsEntity
{
    public:
        LivingEntity(World* world, unsigned int id, sf::Vector2f hitbox_size, float speed);
        virtual ~LivingEntity();

    protected:
        float walking_speed;
        sf::Vector2f walking_direction;

        void walk(float delta);

        inline sf::IntRect getCurrentTextureRect() const { return animation.getCurrentTextureRect(); }

    private:
        Animation<8, 3, 1> animation;
};
