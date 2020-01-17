#include "Player.h"

#include <iostream>
//TEST
//#include <cmath>

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
    #include "../../Networking/ClientToServerCodes.h"
    #include "../../../client-source/States/GameState.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Server/Client.h"
    #include <cstdio>
#endif

#include "../../Blocks/GameBlocks.h"

#include "../../../common-source/Constants.h"
#include "../../Networking/CtoS_PlayerActionCodes.h"
#include "../../Networking/ServerToClientCodes.h"

#ifdef CLIENT_SIDE
unsigned int Player::this_player_id = 0;
Player* Player::this_player = nullptr;

Player::Player(World* world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    inventory(this, world->getState())
{
    if (Player::this_player_id == id)
        Player::this_player = this;

    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, .8f));
    rs.setTexture(&world->getGame()->getResourceManager().getTexture("CHARA_TEST"));

    rs.setFillColor(id == Player::this_player_id ? sf::Color::Green : sf::Color::Red);

    shadow.setRadius(.17f);
    shadow.setOrigin(sf::Vector2f(.17f, .17f));
    shadow.setFillColor(sf::Color(0, 0, 0, 64));
}
#else
Player::Player(World* world, unsigned int id, const Client& client) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    inventory(this, world->getServer()),
    client(client)
{}
#endif

Player::~Player()
{
    //dtor
}

void Player::update(float delta)
{
    #ifdef CLIENT_SIDE
    if (getId() == Player::this_player_id)
    {
        sf::Vector2f dir;

        if (getWorld()->getGame()->getWindow().hasFocus())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            dir += sf::Vector2f(0, -1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                dir += sf::Vector2f(-1.f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                dir += sf::Vector2f(0, 1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                dir += sf::Vector2f(1.f, 0);
        }

        if (dir != last_walking_direction)
        {
            //Send packet to server
            sf::Packet move_packet;
            move_packet << (unsigned short)Networking::CtoS::PlayerAction;
            move_packet << (unsigned short)EntityActions::CtoS::Walk;
            move_packet << dir.x << dir.y;
            move_packet << position.x << position.y;
            getWorld()->getState()->sendToServer(move_packet);

            last_walking_direction = dir;
            setWalkingDirection(dir);
        }
        else
        {
            if (frequent_walk_update.getElapsedTime().asSeconds() >= 1.f)
            {
                frequent_walk_update.restart();

                //We update the server on our movement every second to avoid desynchronisations
                sf::Packet move_packet;
                move_packet << (unsigned short)Networking::CtoS::PlayerAction;
                move_packet << (unsigned short)EntityActions::CtoS::Walk;
                move_packet << dir.x << dir.y;
                move_packet << position.x << position.y;
                getWorld()->getState()->sendToServer(move_packet);
            }
        }
    }

    walk(delta);

    rs.setPosition(position);
    rs.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
    #else
    walk(delta);
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
void Player::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(rs);
}

void Player::moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk)
{
    if (getId() == Player::this_player_id)
        getWorld()->updateChunks(new_chunk);
}
#else
void Player::takePlayerActionPacket(sf::Packet& packet)
{
    unsigned short action; packet >> action;

    if (!packet)
    {
        std::cerr << "Could not read playerAction, packet too short" << std::endl;
        return;
    }

    switch (action)
    {
    case EntityActions::CtoS::Walk:
        {
            sf::Vector2f mov, pos;

            packet >> mov.x >> mov.y;
            packet >> pos.x >> pos.y;

            if (!packet)
            {
                std::cerr << "Could not read playerAction, packet too short" << std::endl;
                break;
            }



            sf::Vector2f diff = pos - position;

            setWalkingDirection(mov);

            if (diff.x * diff.x + diff.y * diff.y >= 1.f)
            {
                sf::Packet rectification;
                rectification << (unsigned short)Networking::StoC::PlayerRectification;
                rectification << position.x << position.y;
                client.send(rectification);
            }
            else
                if (canBeHere(pos))
                    position = pos;
        }
        break;

    case EntityActions::CtoS::PlaceBlock:
        {
            sf::Vector2i pos;
            uint16_t id;

            packet >> pos.x >> pos.y;
            packet >> id;

            if (!packet)
            {
                std::cerr << "Could not read playerAction, packet too short" << std::endl;
                break;
            }

            //std::cout << "Place block at " << pos.x << "; " << pos.y << std::endl;

            if (getWorld()->getBlock(pos) == GameBlocks::AIR)
                getWorld()->setBlock(pos, id);
        }
        break;

    case EntityActions::CtoS::BreakBlock:
        {
            sf::Vector2i pos;

            packet >> pos.x >> pos.y;

            if (!packet)
            {
                std::cerr << "Could not read playerAction, packet too short" << std::endl;
                break;
            }

            auto drops = getWorld()->getBlock(pos)->getDrops();

            for (ItemStack& stack : drops)
                inventory.insertItemStack(stack);

            getWorld()->setBlock(pos, 0);

            inventory.describe();
        }
        break;

    case EntityActions::CtoS::SetGround:
        {
            sf::Vector2i pos;
            uint16_t id;

            packet >> pos.x >> pos.y;
            packet >> id;

            if (!packet)
            {
                std::cerr << "Could not read playerAction, packet too short" << std::endl;
                break;
            }

            getWorld()->setGround(pos, id);
        }
        break;

    default:
        std::cerr << "Could not read playerAction, action code unknown" << std::endl;
        break;
    }
}
#endif

bool Player::isSubscribedTo(sf::Vector2i chunk) const
{
    sf::Vector2i diff = chunk - getChunkOn();
    int distance_squared = diff.x * diff.x + diff.y * diff.y;

    /*
    std::cout << "==========\n" << distance_squared << std::endl;
    std::cout << position.x << ", " << position.y << std::endl;
    std::cout << "==========\n" << std::endl;
    */

    //TODO : Make render distance constant
    return distance_squared < Constants::CHUNK_LOADING_DISTANCE;
}
