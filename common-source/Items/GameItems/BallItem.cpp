#include "BallItem.h"

#include "../ItemStack.h"
#include "../../Entities/GameEntities/Player.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/Server/ClientsManager.h"
    #include "../../../server-source/Packets/InventorySetPacket.h"
#endif // CLIENT_SIDE

BallItem::BallItem() :
    Item("ball", 1)
{

}

BallItem::~BallItem()
{
    //dtor
}

void BallItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
    stack.takeSome(1);

    #ifndef CLIENT_SIDE
    InventorySetPacket set(0, stack.getInt());
    player.getClient().send(set);
    #endif // CLIENT_SIDE
}
