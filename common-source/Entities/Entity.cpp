#include "Entity.h"

Entity::Entity(World* world, unsigned int id) :
    position(0.f, 0.f),
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

#ifdef CLIENT_SIDE
void Entity::draw(sf::RenderTarget& target) const {}
#endif // CLIENT_SIDE
