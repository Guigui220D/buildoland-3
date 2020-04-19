#pragma once

#include "Entity.h"

class PhysicsEntity : public Entity
{
    public:
        PhysicsEntity(World& world, unsigned int id, sf::Vector2f hitbox_size);
        virtual ~PhysicsEntity();

        bool canBeHere(sf::Vector2f new_position) const;
        bool canBlockBeHere(sf::Vector2i block) const;

#ifdef CLIENT_SIDE
        sf::Vector2f getHitbox() const override { return hitbox_size; }
#endif

    protected:
        sf::Vector2f hitbox_size;

    private:

};
