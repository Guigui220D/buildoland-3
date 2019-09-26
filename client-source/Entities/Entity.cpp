#include "Entity.h"

Entity::Entity(World* world, unsigned int id) :
    id(id),
    world(world)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void Entity::update(float delta) {}

void Entity::draw(sf::RenderTarget& target) const {}
