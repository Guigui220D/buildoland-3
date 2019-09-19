#include "TestEntity.h"

TestEntity::TestEntity(unsigned int id, sf::Color col, bool a) :
    Entity(id),
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
        position += sf::Vector2f(0.f, .05f) * delta;
    rs.setPosition(position);
}

void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(rs);
}
