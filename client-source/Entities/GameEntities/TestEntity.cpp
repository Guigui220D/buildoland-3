#include "TestEntity.h"

#include "../../World/World.h"
#include "../../Game.h"

TestEntity::TestEntity(World* world, unsigned int id, bool a) :
    LivingEntity(world, id, sf::Vector2(.5f, .5f), .6f),
    aa(a)
{
    if (aa)
        position = sf::Vector2f(0.f, -2.f);
    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, 1.f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("ANIM_TEST"));
}

TestEntity::~TestEntity()
{
    //dtor
}

void TestEntity::update(float delta)
{
    if (!n_pressed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            n_pressed = true;
    }
    else
    {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            n_pressed = false;
            mode += 1;
            mode %= 5;

            walking_direction = sf::Vector2f();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                walking_direction = sf::Vector2f(0, 1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                walking_direction = sf::Vector2f(1.f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                walking_direction = sf::Vector2f(0, -1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                walking_direction = sf::Vector2f(-1.f, 0);
        }
    }

    walk(delta);

    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());
}

void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(rs);
}
