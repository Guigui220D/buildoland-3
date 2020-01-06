#include "Player.h"

#include <iostream>
//TEST
#include <cmath>

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Server/Client.h"
    #include <cstdio>
#endif

#include "../../../common-source/Constants.h"

#ifdef CLIENT_SIDE
Player::Player(World* world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f)
{

    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, .8f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("CHARA_TEST"));

    shadow.setRadius(.17f);
    shadow.setOrigin(sf::Vector2f(.17f, .17f));
    shadow.setFillColor(sf::Color(0, 0, 0, 64));
}
#else
Player::Player(World* world, unsigned int id, Client& client) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    client(client)
{

}
#endif

Player::~Player()
{
    //dtor
}

void Player::update(float delta)
{
    #ifdef CLIENT_SIDE
    sf::Vector2f dir;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        dir += sf::Vector2f(0, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        dir += sf::Vector2f(-1.f, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        dir += sf::Vector2f(0, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        dir += sf::Vector2f(1.f, 0);

    setWalkingDirection(dir);
    #endif // CLIENT_SIDE

    walk(delta);

    #ifdef CLIENT_SIDE
    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
void Player::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(rs);
}
#endif

bool Player::isSubscribedTo(sf::Vector2i chunk) const
{
    sf::Vector2i diff = chunk - getChunkOn();
    int distance_squared = diff.x * diff.x + diff.y * diff.y;

    /*
    std::cout << "==========\n" << std::sqrt(distance_squared) << std::endl;
    std::cout << chunk->getPos().x << ", " << chunk->getPos().y << std::endl;
    std::cout << position.x << ", " << position.y << std::endl;
    std::cout << chunk->getCenter().x << ", " << chunk->getCenter().y << std::endl;
    std::cout << "==========\n" << std::endl;
    */
    //TODO : Make render distance constant
    return distance_squared < Constants::CHUNK_LOADING_DISTANCE;
}
