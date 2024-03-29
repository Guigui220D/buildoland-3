#include "TestEntity.h"

#include "../../Utils/Log.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/States/GameState.h"
    #include "../../../client-source/World/World.h"
    #include "../../../client-source/Res/ResourceManager.h"
    #include "../../../client-source/Packets/EntityClickPacket.h"
    #include <SFML/Graphics/RenderTarget.hpp>
#else
#endif

TestEntity::TestEntity(World& world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f)
{
    #ifdef CLIENT_SIDE
    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, .8f));
    rs.setTexture(&world.getGame().getResourceManager().getTexture("ZOMBIE"));

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
    walk(delta);

    #ifdef CLIENT_SIDE
    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
    #else
    if (test_clock.getElapsedTime().asSeconds() > 2.f)
    {
        setWalkingDirection(sf::Vector2f(std::rand() % 3 - 1, std::rand() % 3 - 1));
        //setWalkingDirection(sf::Vector2f(0.f, 1.f));
        test_clock.restart();
    }
    #endif // CLIENT_SIDE
}

void TestEntity::onRightClick(Player &player)
{
    (void)player;
#ifdef CLIENT_SIDE
    EntityClickPacket packet(getId(), EntityActions::CtoS::EntityRightClick);
    getWorld().getState().sendToServer(packet);
#else
    log(INFO, "Zombie right click {}\n", getId());
#endif
}

#ifdef CLIENT_SIDE
void TestEntity::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(rs);
}
#endif
