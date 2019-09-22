#pragma once

#include "../LivingEntity.h"

#include "../../Utils/Animation.h"

class TestEntity : public LivingEntity
{
    public:
        TestEntity(World* world, unsigned int id, bool a);
        ~TestEntity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape rs;

        bool n_pressed = false;
        int mode = 0;


        bool aa;
};
