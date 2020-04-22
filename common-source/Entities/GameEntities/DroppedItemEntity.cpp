#include "DroppedItemEntity.h"

#include "../../../common-source/Utils/Log.h"

#ifdef CLIENT_SIDE
#include <SFML/Graphics/RenderTarget.hpp>

#include "../../../client-source/World/World.h"
#include "../../../client-source/Game.h"
#include "../../../client-source/Res/ResourceManager.h"
#include "../../../client-source/Packets/EntityClickPacket.h"
#include "../../../client-source/States/GameState.h"
#else
#include "../../../common-source/Entities/GameEntities/Player.h"

#include <cassert>
#endif

#ifndef CLIENT_SIDE
DroppedItemEntity::DroppedItemEntity(World &world, unsigned int id, sf::Vector2f in_position)
    : Entity(world, id)
{
    position = in_position;
}
#else
DroppedItemEntity::DroppedItemEntity(World &world, unsigned int id)
    : Entity(world, id)
{
    number_text.setFont(getWorld().getGame().getResourceManager().getFont("GUI_FONT"));
    number_text.setString("foo");
    number_text.setCharacterSize(100);
    number_text.scale(sf::Vector2f(1.f/350, 1.f/350));
    number_text.setOutlineColor(sf::Color::Black);
    number_text.setOutlineThickness(4.f);

    shadow.setRadius(.20f);
    shadow.setScale(1.1f, 1.0f);
    shadow.setOrigin(shadow.getLocalBounds().width/2.f, shadow.getLocalBounds().height/2.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 90));
}
#endif

DroppedItemEntity::~DroppedItemEntity()
{

}

void DroppedItemEntity::setItemStack(ItemStack &input_stack)
{
    stack.swap(input_stack);

#ifdef CLIENT_SIDE
    sf::Texture const * block_textures;
    sf::Texture const * ground_textures;
    sf::Texture const * item_textures;

    block_textures = &getWorld().getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getWorld().getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    item_textures = &getWorld().getGame().getResourceManager().getTexture("ITEM_TEXTURES");

    switch (stack.getItem()->getTexturesSet())
    {
        case Item::BlocksTextureSet:
            item_sprite.setTexture(*block_textures);
            break;
        case Item::GroundsTextureSet:
            item_sprite.setTexture(*ground_textures);
            break;
        case Item::ItemsTextureSet:
            item_sprite.setTexture(*item_textures);
            break;
        default:;
    }

    item_sprite.setTextureRect(stack.getItem()->getTexture(stack));
    item_sprite.setScale(1.0f/item_sprite.getTextureRect().width / 2.f, 1.0f/item_sprite.getTextureRect().height / 2.f);
    item_sprite.setOrigin(item_sprite.getTextureRect().width / 2.f,
                          item_sprite.getTextureRect().height / 2.f);

    sprite_outline.setRadius(0.6f/2.f);
    sprite_outline.setPointCount(32);
    sprite_outline.setFillColor(sf::Color::Transparent);
    sprite_outline.setOutlineColor(sf::Color::White);
    sprite_outline.setOutlineThickness(0.05f);
    sprite_outline.setOrigin(0.6f/2.f,
                             0.6f/2.f);

    number_text.setString(std::to_string(stack.getAmount()));
#endif
}

void DroppedItemEntity::update(float delta)
{
#ifdef CLIENT_SIDE
    floating_anim_progress += delta;

    item_sprite.setPosition(getPosition().x, getPosition().y + sinf(floating_anim_progress*4.f)/16.f);
    sprite_outline.setPosition(item_sprite.getPosition());
    shadow.setPosition(getPosition() + sf::Vector2f(0.0f, 0.25f));
    number_text.setPosition(getPosition().x + 1.0f/item_sprite.getTextureRect().width*2.f,
                            getPosition().y + 1.0f/item_sprite.getTextureRect().height*2.0f + sinf(floating_anim_progress*4.f)/16.f);
#endif
}

void DroppedItemEntity::onLeftClick(Player& player)
{
#ifdef CLIENT_SIDE
    EntityClickPacket packet(getId(), EntityActions::CtoS::EntityLeftClick);
    getWorld().getState().sendToServer(packet);
#else
    //log(INFO, "Stack left click {}\n", getId());
    assert(stack);
    if (player.getInventory().insertItemStack(stack, false))
        to_be_removed = true;
#endif
}

#ifdef CLIENT_SIDE
bool DroppedItemEntity::takeNewEntityPacket(ECCPacket &packet)
{
    sf::Vector2f new_pos;

    packet >> new_pos.x;
    packet >> new_pos.y;

    uint32_t stack_val;
    packet >> stack_val;
    if (!packet)
    {
        log(ERROR, "Dropped Item entity packet was too short.\n");
        return false;
    }
    position = new_pos;

    ItemStack new_stack(stack_val, getWorld().getGame().getItemsRegister());
    setItemStack(new_stack);

    return true;
}

void DroppedItemEntity::drawBelow(sf::RenderTarget &target) const
{
    target.draw(shadow);
}

void DroppedItemEntity::draw(sf::RenderTarget &target) const
{
    //target.draw(sprite_outline);
    target.draw(item_sprite);

    if (isHighlighted())
    {
        target.draw(number_text);
    }
}

#else
void DroppedItemEntity::addInfoToNewEntityPacket(ECCPacket& packet) const
{
    packet << position.x;
    packet << position.y;
    packet << stack.getInt();
}
#endif
