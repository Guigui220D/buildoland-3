#include "TestEntity.h"

#include "../../World/World.h"
#include "../../Game.h"

TestEntity::TestEntity(World* world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2(.5f, .5f), 2.f)
{
    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, 1.f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("ANIM_TEST"));

    shadow.setRadius(.5f);
    shadow.setOrigin(sf::Vector2(.5f, .5f));
    shadow.setFillColor(sf::Color(0, 0, 0, 127));
}

TestEntity::~TestEntity()
{
    //dtor
}

void TestEntity::update(float delta)
{
    walking_direction = sf::Vector2f();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        walking_direction += sf::Vector2f(0, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        walking_direction += sf::Vector2f(-1.f, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        walking_direction += sf::Vector2f(0, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        walking_direction += sf::Vector2f(1.f, 0);

    walk(delta);

    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
}

void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(rs);
}
