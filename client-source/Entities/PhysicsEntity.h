#pragma once

#include "Entity.h"

class PhysicsEntity : public Entity
{
    public:
        PhysicsEntity(World* world, unsigned int id, sf::Vector2f hitbox_size);
        virtual ~PhysicsEntity();

        bool canBeHere(sf::Vector2f new_position) const;

        inline sf::Vector2f getHitbox() const { return hitbox_size; }

    protected:
        sf::Vector2f hitbox_size;

    private:

};
