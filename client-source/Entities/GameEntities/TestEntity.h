#pragma once

#include "../PhysicsEntity.h"

class TestEntity : public PhysicsEntity
{
    public:
        TestEntity(World* world, unsigned int id, sf::Color col, bool a);
        ~TestEntity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape rs;

        bool aa;
};
