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

    base.setSize(sf::Vector2f(1.f, 1.f)); base.setOrigin(sf::Vector2f(.5f, .75f));

    shirt = pants = shoes = base;

    base.setTexture(&world.getGame().getResourceManager().getTexture("CHARA_BASE"));
    pants.setTexture(&world.getGame().getResourceManager().getTexture("CHARA_PANTS"));
    shoes.setTexture(&world.getGame().getResourceManager().getTexture("CHARA_SHOES"));
    shirt.setTexture(&world.getGame().getResourceManager().getTexture("CHARA_SHIRT"));

    shadow.setRadius(.17f);
    shadow.setOrigin(sf::Vector2f(.17f, .17f));
    shadow.setFillColor(sf::Color(0, 0, 0, 64));
}
#else
Player::Player(World& world, unsigned int id, const Client& client) :
    LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
    inventory(*this, world.getServer()),
    client(client)
{
    for (uint32_t& color : outfit_colors)
        color = ((std::rand() & 0xFF) << 24) | ((std::rand() & 0xFF) << 16) | ((std::rand() & 0xFF) << 8) | 0xFF;
}
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            dir += sf::Vector2f(0, -1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                dir += sf::Vector2f(-1.f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                dir += sf::Vector2f(0, 1.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                dir += sf::Vector2f(1.f, 0);
        }

        if (dir != last_walking_direction)
        {
            //Send packet to server
            ECCPacket move_packet;
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
                ECCPacket move_packet;
                move_packet << Networking::CtoS::PlayerAction;
                move_packet << EntityActions::CtoS::Walk;
                move_packet << dir.x << dir.y;
                move_packet << position.x << position.y;
                getWorld().getState().sendToServer(move_packet);
            }
        }
    }

    walk(delta);

    base.setPosition(position);
    shirt.setPosition(position);
    pants.setPosition(position);
    shoes.setPosition(position);

    base.setTextureRect(getCurrentTextureRect());
    shirt.setTextureRect(getCurrentTextureRect());
    pants.setTextureRect(getCurrentTextureRect());
    shoes.setTextureRect(getCurrentTextureRect());

    shadow.setPosition(position);
    #else
    walk(delta);
    #endif // CLIENT_SIDE
}

#ifdef CLIENT_SIDE
void Player::draw(sf::RenderTarget& target) const
{
    target.draw(shadow);
    target.draw(base);
    target.draw(shoes);
    target.draw(pants);
    target.draw(shirt);
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

bool Player::takeNewEntityPacket(ECCPacket& packet)
{
    for (uint32_t& color : outfit_colors)
        packet >> color;

    if (packet)
    {
        shoes.setFillColor(sf::Color(outfit_colors[0]));
        pants.setFillColor(sf::Color(outfit_colors[1]));
        shirt.setFillColor(sf::Color(outfit_colors[2]));
    }

    return true;
}
#else
void Player::takePlayerActionPacket(ECCPacket& packet)
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
                ECCPacket rectification;
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

            if (!isSubscribedTo(World::getChunkPosFromBlockPos(pos)))
                return;

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

            if (!isSubscribedTo(World::getChunkPosFromBlockPos(pos)))
                return;

            auto drops = getWorld().getBlock(pos)->getDrops();

            for (ItemStack& stack : drops)
                inventory.insertItemStack(stack);

            getWorld().setBlock(pos, GameBlocks::AIR);
        }
        break;

    case EntityActions::CtoS::SwapInventoryItem:
        {
            int pos; packet >> pos;

            if (!packet)
                break;

            inventory.swapHands(pos);
        }
        break;

    default:
        std::cerr << "Could not read playerAction, action code unknown" << std::endl;
        break;
    }
}

void Player::makeNewEntityPacket(ECCPacket& packet) const
{
    packet.clear();

    packet << Networking::StoC::EntityAction;
    packet << EntityActions::StoC::AddEntity;
    packet << getEntityCode();
    packet << getId();

    for (uint32_t color : outfit_colors)
        packet << color;

    addInfoToNewEntityPacket(packet);
}
#endif

bool Player::isSubscribedTo(sf::Vector2i chunk, bool twice) const
{
    sf::Vector2i diff = chunk - getChunkOn();
    int distance_squared = diff.x * diff.x + diff.y * diff.y;

    //TODO : Make render distance constant
    return distance_squared < (Constants::CHUNK_LOADING_DISTANCE + Constants::CHUNK_LOADING_DISTANCE * twice);
}
