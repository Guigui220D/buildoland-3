#pragma once

#include "../PhysicsEntity.h"

#include "../../Utils/Animation.h"

class TestEntity : public PhysicsEntity
{
    public:
        TestEntity(World* world, unsigned int id, bool a);
        ~TestEntity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

    private:
        Animation<8, 3, 1> anim;
        int animation = 0;

        sf::RectangleShape rs;

        bool n_pressed = false;
        bool aa;
};
