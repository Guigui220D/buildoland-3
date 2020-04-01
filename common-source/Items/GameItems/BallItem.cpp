#include "BallItem.h"

#include "../ItemStack.h"
#include "../../Entities/GameEntities/Player.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/Server/ClientsManager.h"
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
    ECCPacket set;

    set << Networking::StoC::InventoryUpdate;
    set << InventoryUpdates::StoC::SetStack;
    set << 0;   //We set the first slot of the inventory (hand)
    set << stack.getInt();

    player.getClient().send(set);
    #endif // CLIENT_SIDE
}
