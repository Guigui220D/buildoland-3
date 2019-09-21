#include "TestEntity.h"

TestEntity::TestEntity(World* world, unsigned int id, sf::Color col, bool a) :
    PhysicsEntity(world, id, sf::Vector2(1.f, 1.f)),
    aa(a)
{
    rs.setFillColor(col);
    if (aa)
        position = sf::Vector2f(0.f, -2.f);
    rs.setSize(sf::Vector2f(.8f, .8f));
    rs.setOrigin(sf::Vector2f(.4f, .8f));
}

TestEntity::~TestEntity()
{
    //dtor
}

void TestEntity::update(float delta)
{
    if (aa)
    {
        if (canBeHere(position + sf::Vector2f(0.f, .6f) * delta))
            position += sf::Vector2f(0.f, .6f) * delta;
    }
    rs.setPosition(position);
}

void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(rs);
}
