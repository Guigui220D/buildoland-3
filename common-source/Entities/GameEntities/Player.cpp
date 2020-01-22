#include "Player.h"

#include <iostream>
//TEST
//#include <cmath>

#include "../../Networking/NetworkingCodes.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
    #include "../../../client-source/States/GameState.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Server/Client.h"
    #include <cstdio>
#endif

#include "../../Blocks/GameBlocks.h"

#include "../../../common-source/Constants.h"

#ifdef CLIENT_SIDE
unsigned int Player::this_player_id = 0;
Player* Player::this_player = nullptr;

Player::Player(World& world, unsigned int id) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    inventory(*this, world.getState())
{
    if (Player::this_player_id == id)
        Player::this_player = this;

    sf::RenderTexture chara_render; chara_render.create(96, 256);

    chara_render.clear(sf::Color::Transparent);

    {
        //TODO : Put that somewhere else
        sf::Sprite base; base.setTexture(world.getGame().getResourceManager().getTexture("CHARA_BASE"));
        sf::Sprite shoes; shoes.setTexture(world.getGame().getResourceManager().getTexture("CHARA_SHOES"));
        sf::Sprite pants; pants.setTexture(world.getGame().getResourceManager().getTexture("CHARA_PANTS"));
        sf::Sprite shirt; shirt.setTexture(world.getGame().getResourceManager().getTexture("CHARA_SHIRT"));

        sf::Color shoes_color = sf::Color(world.getGame().getSettingsManager().getInt("player_shoes_color")); shoes_color.a = 255; shoes.setColor(shoes_color);
        sf::Color pants_color = sf::Color(world.getGame().getSettingsManager().getInt("player_pants_color")); pants_color.a = 255; pants.setColor(pants_color);
        sf::Color shirt_color = sf::Color(world.getGame().getSettingsManager().getInt("player_shirt_color")); shirt_color.a = 255; shirt.setColor(shirt_color);

        chara_render.draw(base);
        chara_render.draw(shoes);
        chara_render.draw(pants);
        chara_render.draw(shirt);
    }

    chara_render.display();
    texture = chara_render.getTexture();

    rs.setSize(sf::Vector2f(1.f, 1.f));
    rs.setOrigin(sf::Vector2f(.5f, .8f));
    rs.setTexture(&texture);

    shadow.setRadius(.17f);
    shadow.setOrigin(sf::Vector2f(.17f, .17f));
    shadow.setFillColor(sf::Color(0, 0, 0, 64));
}
#else
Player::Player(World& world, unsigned int id, const Client& client) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    inventory(*this, world.getServer()),
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

        if (getWorld().getGame().getWindow().hasFocus())
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
            move_packet << Networking::CtoS::PlayerAction;
            move_packet << EntityActions::CtoS::Walk;
            move_packet << dir.x << dir.y;
            move_packet << position.x << position.y;
            getWorld().getState().sendToServer(move_packet);

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
                move_packet << Networking::CtoS::PlayerAction;
                move_packet << EntityActions::CtoS::Walk;
                move_packet << dir.x << dir.y;
                move_packet << position.x << position.y;
                getWorld().getState().sendToServer(move_packet);
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
        getWorld().updateChunks(new_chunk);
}

void Player::useHand(sf::Vector2i pos)
{
    inventory.contents.at(0).getItem()->useItem(inventory.contents.at(0), getWorld(), pos, *this);
}
#else
void Player::takePlayerActionPacket(sf::Packet& packet)
{
    int action; packet >> action;

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
                rectification << Networking::StoC::PlayerRectification;
                rectification << position.x << position.y;
                client.send(rectification);
            }
            else
                if (canBeHere(pos))
                    position = pos;
        }
        break;

    case EntityActions::CtoS::UseItem:
        {
            sf::Vector2i pos;

            packet >> pos.x >> pos.y;

            if (!packet)
            {
                std::cerr << "Could not read playerAction, packet too short" << std::endl;
                break;
            }

            inventory.contents.at(0).getItem()->useItem(inventory.contents.at(0), getWorld(), pos, *this);
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

            auto drops = getWorld().getBlock(pos)->getDrops();

            for (ItemStack& stack : drops)
                inventory.insertItemStack(stack);

            getWorld().setBlock(pos, 0);
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

    //TODO : Make render distance constant
    return distance_squared < Constants::CHUNK_LOADING_DISTANCE;
}
