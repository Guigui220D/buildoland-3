#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Utils/Animation.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#else
#include <SFML/System/Clock.hpp>
#endif

class Chunk;

class TestEntity : public LivingEntity
{
    public:
        inline unsigned short getEntityCode() const override { return Entities::TestEntity; };

        TestEntity(World& world, unsigned int id);
        ~TestEntity();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;
        sf::Vector2f getVisualHitbox() const override
        { return sf::Vector2f(.5f, 1.0f); }
        #endif

        virtual void onRightClick(Player& player);

    private:
        #ifdef CLIENT_SIDE
        sf::Texture texture;
        sf::RectangleShape rs;
        sf::CircleShape shadow;
        #else
        sf::Clock test_clock;
        #endif
};
