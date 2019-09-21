#pragma once

#include <SFML/Graphics.hpp>

class World;

class Entity
{
    public:
        Entity(World* world, unsigned int id);
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
        inline World* getWorld() const { return world; }

    protected:
        sf::Vector2f position;

    private:
        const unsigned int id;
        World* const world;
};
