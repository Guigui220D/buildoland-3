#include "TestEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
#else
    #include "../../../server-source/World/World.h"
#endif

TestEntity::TestEntity(World* world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f)
{
    #ifdef CLIENT_SIDE
    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, .8f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("CHARA_TEST"));

    shadow.setRadius(.17f);
    shadow.setOrigin(sf::Vector2f(.17f, .17f));
    shadow.setFillColor(sf::Color(0, 0, 0, 64));
    #endif
}

TestEntity::~TestEntity()
{
    //dtor
}

void TestEntity::update(float delta)
{
    walking_direction = sf::Vector2f();

    walk(delta);

    #ifdef CLIENT_SIDE
    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(rs);
}
#endif
