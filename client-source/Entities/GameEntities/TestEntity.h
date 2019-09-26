#pragma once

#include "../LivingEntity.h"

#include "../../Utils/Animation.h"

class TestEntity : public LivingEntity
{
    public:
        TestEntity(World* world, unsigned int id);
        ~TestEntity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape rs;
        sf::CircleShape shadow;
};
