#include "ShovelItem.h"

#include "../ItemStack.h"
#include "../../Entities/GameEntities/Player.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/Server/ClientsManager.h"
    #include "../../../server-source/World/World.h"
#endif // CLIENT_SIDE

#include <iostream>

ShovelItem::ShovelItem() :
    Item("shovel", 2)
{

}

ShovelItem::~ShovelItem()
{
    //dtor
}

void ShovelItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
    #ifndef CLIENT_SIDE
    auto drops = world.getGround(click_pos)->getDrops();

    for (ItemStack& stack : drops)
        player.getInventory().insertItemStack(stack);

    world.setGround(click_pos, GameGrounds::ERROR->getId());
    #endif // CLIENT_SIDE
}
