#pragma once

#include "../Entity.h"

#include "../../Items/ItemStack.h"

#ifdef CLIENT_SIDE
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#endif

#include <memory>

class DroppedItemEntity : public Entity
{
public:
#ifndef CLIENT_SIDE
    DroppedItemEntity(World& world, unsigned int id);
#else
    DroppedItemEntity(World& world, unsigned int id);
#endif
    ~DroppedItemEntity();

    void setItemStack(ItemStack& input_stack);

    unsigned short getEntityCode() const override { return Entities::DroppedItemEntity; };

#ifdef CLIENT_SIDE
    bool takeNewEntityPacket(ECCPacket& packet) override;
    sf::Vector2f getHitbox() const override { return {0.5f, 0.5f}; }
    int getZOrder() const override
    { return 2; } // draw the items on top of all entities
    void drawBelow(sf::RenderTarget& target) const override ;
    void draw(sf::RenderTarget& target) const override ;
#else
    void addInfoToNewEntityPacket(ECCPacket& packet) const override;

    virtual nlohmann::json* serializeToJson() const;
    virtual void deserialize(nlohmann::json& json) override;
#endif

    void onLeftClick(Player &player) override;

protected:
    void update(float delta) override;

private:
    ItemStack stack;
#ifdef CLIENT_SIDE
    sf::Sprite item_sprite;
    sf::CircleShape sprite_outline;
    sf::CircleShape shadow;
    mutable sf::Text number_text;
    float floating_anim_progress = 0.f;
#endif
};
