#include "Entity.h"

#include "../../server-source/World/World.h"

#include <iostream>

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

void Entity::updateBase(float delta)
{
    update(delta);

    sf::Vector2i co = getChunkOn();
    if (co != chunk_on)
    {
        chunk_on = co;
        onChunkChange();
    }
}

void Entity::update(float delta) {}

#ifdef CLIENT_SIDE
void Entity::draw(sf::RenderTarget& target) const {}
#endif // CLIENT_SIDE

sf::Vector2i Entity::getChunkOn() const
{
    return World::getChunkPosFromBlockPos(getBlockOn());
}

void Entity::onChunkChange()
{
    std::cout << "Entity " << getId() << " changed chunk. " << std::endl;
}
