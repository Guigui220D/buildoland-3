#include "Entity.h"

Entity::Entity(unsigned int id) :
    id(id)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void Entity::update(float delta) {}

void Entity::draw(sf::RenderTarget& target) const {}
