#pragma once

#include "PhysicsEntity.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/Utils/Animation.h"
#endif // CLIENT_SIDE

class LivingEntity : public PhysicsEntity
{
    public:
        LivingEntity(World& world, unsigned int id, sf::Vector2f hitbox_size, float speed);
        virtual ~LivingEntity();

    protected:
        enum : unsigned short
        {
            Walk,
        };  //Actions

        inline float getWalkingSpeed() const { return walking_speed; }
        inline void setWalkingSpeed(float speed) { walking_speed = speed; }

        void setWalkingDirection(sf::Vector2f new_direction);

        void walk(float delta);

        #ifdef CLIENT_SIDE
        inline sf::IntRect getCurrentTextureRect() const { return animation.getCurrentTextureRect(); }

        bool takePacket(sf::Packet& packet) override;

        bool takeNewEntityPacket(sf::Packet& packet) override;
        #else
        void addInfoToNewEntityPacket(sf::Packet& packet) const override;
        #endif
    private:
        float walking_speed;
        sf::Vector2f walking_direction;

        #ifdef CLIENT_SIDE
        Animation animation;
        #endif
};
