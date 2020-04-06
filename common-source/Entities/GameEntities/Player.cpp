#include "Player.h"

#include <iostream>
//TEST
//#include <cmath>

#include "../../Networking/NetworkingCodes.h"

#ifdef CLIENT_SIDE
#include "../../../client-source/Game.h"
#include "../../../client-source/World/World.h"
#include "../../../client-source/States/GameState.h"

#include "../../../client-source/Packets/WalkPacket.h"
#else
#include "../../../server-source/World/World.h"
#include "../../../server-source/Server/Client.h"
#include "../../../server-source/Packets/PlayerRectificationPacket.h"
#include "../../../server-source/Packets/FullInventoryPacket.h"
#include "../../../server-source/Server/Server.h"
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
            if (getWorld().getGame().getBindingsManager().held("move_up"))
                dir += sf::Vector2f(0, -1.f);
            if (getWorld().getGame().getBindingsManager().held("move_left"))
                dir += sf::Vector2f(-1.f, 0);
            if (getWorld().getGame().getBindingsManager().held("move_down"))
                dir += sf::Vector2f(0, 1.f);
            if (getWorld().getGame().getBindingsManager().held("move_right"))
                dir += sf::Vector2f(1.f, 0);
        }

        if (dir != last_walking_direction)
        {
            //Send packet to server
            WalkPacket move_packet(dir, position);
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
                WalkPacket move_packet(dir, position);
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
void Player::handlePlayerActionRequest(const Networking::CtoS::PlayerActionRequest& rq)
{
    switch (rq.type)
    {
        case EntityActions::CtoS::Walk:
        {
            sf::Vector2f walk_pos(rq.walk_pos_x, rq.walk_pos_y);
            sf::Vector2f walk_mov(rq.walk_mov_x, rq.walk_mov_y);

            sf::Vector2f diff = walk_pos - position;

            setWalkingDirection(walk_mov);

            if (diff.x * diff.x + diff.y * diff.y >= 1.f)
            {
                PlayerRectificationPacket rectification(position);
                client.send(rectification);

                std::vector<sf::Vector2i> sent;

                for (int i = -1; i <= 1; i += 2)
                for (int j = -1; j <= 1; j += 2)
                {
                    sf::Vector2i cpos = World::getChunkPosFromBlockPos(getBlockOn() + sf::Vector2i(i, j));

                    if (std::find(sent.begin(), sent.end(), cpos) == sent.end())
                    {
                        sent.push_back(cpos);
                        getClient().send(getWorld().getChunk(getChunkOn()).getPacket());
                    }
                }
            }
            else if (canBeHere(walk_pos))
                position = walk_pos;
        }
        break;

        case EntityActions::CtoS::UseItem:
        {
            sf::Vector2i item_pos(rq.item_pos_x, rq.item_pos_y);

            {
                ItemStack hand(rq.item_in_hand, getWorld().getServer().getItemsRegister());

                if (hand.getItem() != inventory.contents.at(0).getItem())
                {
                    bool has_item = false;
                    for (int i = 1; i < 25; i++)
                    {
                        ItemStack& is = inventory.contents.at(i);
                        if (!is)
                            continue;
                        if (is.getItem() == hand.getItem())
                        {
                            inventory.swapHands(i);
                            has_item = true;
                            break;
                        }
                    }

                    FullInventoryPacket fip(inventory);
                    getClient().send(fip);
                    if (!has_item)
                        break;
                    //std::clog << "TEST : Player used wrong item but that wasn't cheating, rectifying. " << std::endl;
                }
            }

            if (!isSubscribedTo(World::getChunkPosFromBlockPos(item_pos)))
                break;

            inventory.contents.at(0).getItem()->useItem(inventory.contents.at(0), getWorld(), item_pos, *this);
        }
        break;

        case EntityActions::CtoS::BreakBlock:
        {
            if (!isSubscribedTo(World::getChunkPosFromBlockPos(rq.break_block_pos)))
                break;

            auto drops = getWorld().getBlock(rq.break_block_pos)->getDrops();

            for (ItemStack& stack : drops)
                inventory.insertItemStack(stack);

            getWorld().setBlock(rq.break_block_pos, GameBlocks::AIR);
        }
        break;

        case EntityActions::CtoS::SwapInventoryItem:
        {
            ItemStack hand(rq.hand_item, getWorld().getServer().getItemsRegister());
            ItemStack slot(rq.slot_item, getWorld().getServer().getItemsRegister());

            if (hand && slot && (inventory.contents.at(0).getItem() != hand.getItem() || inventory.contents.at(rq.item_swap_pos).getItem() != slot.getItem()))
            {
                if (inventory.contents.at(0).getItem() == slot.getItem() && inventory.contents.at(rq.item_swap_pos).getItem() == hand.getItem())
                    break;

                FullInventoryPacket fip(inventory);
                getClient().send(fip);
                break;
            }

            inventory.swapHands(rq.item_swap_pos);
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
