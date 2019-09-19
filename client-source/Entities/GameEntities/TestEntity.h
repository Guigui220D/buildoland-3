#pragma once

#include "../Entity.h"

class TestEntity : public Entity
{
    public:
        TestEntity(unsigned int id, sf::Color col, bool a);
        ~TestEntity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape rs;

        bool aa;
};
