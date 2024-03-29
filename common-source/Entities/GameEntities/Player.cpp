#include "Player.h"

#include <algorithm>
#include <exception>

#include "../../Networking/NetworkingCodes.h"

#ifdef CLIENT_SIDE
    #include <SFML/Window/Keyboard.hpp>
    #include <SFML/Graphics/RenderWindow.hpp>

    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
    #include "../../../client-source/States/GameState.h"
    #include "../../../client-source/Packets/BreakBlockPacket.h"
    #include "../../../client-source/Packets/WalkPacket.h"
    #include "../../../client-source/Res/ResourceManager.h"
    #include "../../../client-source/Settings/BindingsManager.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Packets/PlayerRectificationPacket.h"
    #include "../../../server-source/Packets/FullInventoryPacket.h"
    #include "../../../server-source/Server/Server.h"
    #include "../../../server-source/World/Chunk.h"
    #include "../../../server-source/World/EntitiesManager.h"
    #include "../../../external/json/Json.hpp"
#endif

#include "../../Blocks/Block.h"
#include "../../Blocks/GameBlocks.h"

#include "../../../common-source/Constants.h"
#include "../../../common-source/Utils/Log.h"

#ifdef CLIENT_SIDE
unsigned int Player::this_player_id = 0;
Player* Player::this_player = nullptr;

Player::Player(World& world, unsigned int id) :
      LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
      inventory(*this)
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

    nickname_text.setFont(getWorld().getGame().getResourceManager().getFont("GUI_FONT"));
    nickname_text.setString("Me");
    nickname_text.setCharacterSize(100);
    nickname_text.scale(sf::Vector2f(1.f/300, 1.f/300));
    nickname_text.setOutlineColor(sf::Color::Black);
    nickname_text.setOutlineThickness(4.f);
}
#else
Player::Player(World& world, unsigned int id, const Client& client) :
      LivingEntity(world, id, sf::Vector2f(.5f, .5f), 3.f),
      inventory(*this),
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
        autowalk = true;


    if (autowalk && aw_break.getElapsedTime().asSeconds() >= .5f)
    {
        BreakBlockPacket bp(getBlockOn() + sf::Vector2i(1, 0));
        getWorld().getState().sendToServer(bp);
        aw_break.restart();
    }

    if (getId() == Player::this_player_id)
    {
        sf::Vector2f dir;

        if (getWorld().getGame().getWindow().hasFocus() && !getWorld().getState().typingInChat())
        {
            if (getWorld().getGame().getBindingsManager().held("move_up"))
                dir += sf::Vector2f(0, -1.f);
            if (getWorld().getGame().getBindingsManager().held("move_left"))
                dir += sf::Vector2f(-1.f, 0);
            if (getWorld().getGame().getBindingsManager().held("move_down"))
                dir += sf::Vector2f(0, 1.f);
            if (getWorld().getGame().getBindingsManager().held("move_right") || autowalk)
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
    nickname_text.setPosition(position + sf::Vector2f(0.0f, -1.2f));

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

void Player::drawAbove(sf::RenderTarget &target) const
{
#ifndef SOLO
    target.draw(nickname_text);
#endif
}

void Player::moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk)
{
    if (getId() == Player::this_player_id)
        getWorld().updateChunks(new_chunk);
}

void Player::useHand(sf::Vector2i pos)
{
    inventory.getHand().getItem()->useItem(inventory.getHand(), getWorld(), pos, *this);
}

bool Player::takeNewEntityPacket(ECCPacket& packet)
{
    std::string nick;
    packet >> nick;

    log(INFO, "Player nickname : {}\n", nick);
    nickname_text.setString(nick);
    nickname_text.setOrigin(nickname_text.getLocalBounds().width/2.f, 0.f);

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
                    const sf::Vector2i cpos = World::getChunkPosFromBlockPos(getBlockOn() + sf::Vector2i(i, j));

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

                if (hand.getItem() != inventory.getHand().getItem())
                {
                    bool has_item = false;
                    for (int i = 1; i < 25; i++)
                    {
                        ItemStack& is = inventory.getStack(i);
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
                }
            }

            if (!isSubscribedTo(World::getChunkPosFromBlockPos(item_pos)))
                break;

            inventory.getHand().getItem()->useItem(inventory.getHand(), getWorld(), item_pos, *this);
        }
        break;

        case EntityActions::CtoS::BreakBlock:
        {
            if (!isSubscribedTo(World::getChunkPosFromBlockPos(rq.break_block_pos)))
                break;

            auto drops = getWorld().getBlockPtr(rq.break_block_pos)->getDrops();

            for (ItemStack& stack : drops)
                inventory.insertItemStack(stack, sf::Vector2f(rq.break_block_pos.x, rq.break_block_pos.y));

            getWorld().setBlock(rq.break_block_pos, GameBlocks::AIR);
        }
        break;

        case EntityActions::CtoS::SwapInventoryItem:
        {
            ItemStack hand(rq.hand_item, getWorld().getServer().getItemsRegister());
            ItemStack slot(rq.slot_item, getWorld().getServer().getItemsRegister());

            if (hand && slot && (inventory.getHand().getItem() != hand.getItem() || inventory.getStack(rq.item_swap_pos).getItem() != slot.getItem()))
            {
                if (inventory.getHand().getItem() == slot.getItem() && inventory.getStack(rq.item_swap_pos).getItem() == hand.getItem())
                    break;

                FullInventoryPacket fip(inventory);
                getClient().send(fip);
                break;
            }

            inventory.swapHands(rq.item_swap_pos);
        }
        break;

        case EntityActions::CtoS::DropInventoryItem:
        {
            ItemStack hand(rq.hand_item, getWorld().getServer().getItemsRegister());

            if (hand && (inventory.getHand().getItem() != hand.getItem()))
            {
                FullInventoryPacket fip(inventory);
                getClient().send(fip);
                break;
            }

            inventory.dropHand();
        }
        break;

        case EntityActions::CtoS::EntityLeftClick:
        case EntityActions::CtoS::EntityRightClick:
        {
            Entity* entity = getWorld().getEntityManager().getEntity(rq.entity_id);
            if (!entity)
                break;

            if (!isSubscribedTo(entity->getChunkOn()))
                break;

            if (rq.type == EntityActions::CtoS::EntityLeftClick)
                entity->onLeftClick(*this);
            else
            {
                log(INFO," server right click {}\n", rq.entity_id);
                entity->onRightClick(*this);
            }
        }
        break;

        default:
            log(ERROR, "Could not read playerAction, action code unknown\n");
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

    packet << client.getNickname();

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

#ifndef CLIENT_SIDE
nlohmann::json* Player::serializeToJson() const
{
    nlohmann::json* json = new nlohmann::json();
    (*json)["pos_x"] = position.x;
    (*json)["pos_y"] = position.y;
    (*json)["player"] = getClient().getNickname();
    /*
    auto inv = inventory.serializeToJson();
    (*json)["inventory"] = *inv;
    delete inv;
    */
    //We intentionally don't serialize player with their entity type, won't don't actually care about saving them
    return json;
}

void Player::deserialize(nlohmann::json& json)
{
    throw std::logic_error("Player can't be deserialized! It is not meant to be loaded that way.");
}
#endif // CLIENT_SIDE
