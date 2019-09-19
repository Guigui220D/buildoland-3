#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
    public:
        Entity(unsigned int id);
        virtual ~Entity();

        void update(float delta);
        void draw(sf::RenderTarget& target) const;

        inline unsigned int getId() const { return id; }

    protected:
        sf::Vector2f position;

    private:
        const unsigned int id;
};
