#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
    public:
        Entity(unsigned int id);
        virtual ~Entity();

        /**
         * Updates this entity
         * @param delta : time since last update
         */
        virtual void update(float delta);
        /**
         * Draw this entity
         * @param target : the render target on which to draw
         */
        virtual void draw(sf::RenderTarget& target) const;

        inline unsigned int getId() const { return id; }

        inline sf::Vector2f getPosition() const { return position; }

    protected:
        sf::Vector2f position;

    private:
        const unsigned int id;
};
