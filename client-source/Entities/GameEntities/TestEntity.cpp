#include "TestEntity.h"

#include "../../World/World.h"
#include "../../Game.h"

TestEntity::TestEntity(World* world, unsigned int id, bool a) :
    PhysicsEntity(world, id, sf::Vector2(1.f, 1.f)),
    anim(.2f),
    aa(a)
{
    if (aa)
        position = sf::Vector2f(0.f, -2.f);
    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, 1.f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("ANIM_TEST"));

    anim.addAnimation({ 1, 0, 1, 2 });
    anim.addAnimation({ 4, 3, 4, 5 });
    anim.addAnimation({ 7, 6, 7, 8 });
    anim.addAnimation({ 10, 9, 10, 11 });
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
            animation += 1;
            animation %= 4;
            anim.selectAnimation(animation);
        }
    }

    if (aa)
    {
        if (canBeHere(position + sf::Vector2f(0.f, .6f) * delta))
            position += sf::Vector2f(0.f, .6f) * delta;
    }
    rs.setPosition(position);

    rs.setTextureRect(anim.getCurrentTextureRect());
}

void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(rs);
}
