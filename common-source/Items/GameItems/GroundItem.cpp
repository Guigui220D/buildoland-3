#include "GroundItem.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/World/World.h"
#else
    #include "../../../server-source/World/World.h"
#endif // CLIENT_SIDE

#include "../../Grounds/GameGrounds.h"

#include "../ItemStack.h"

GroundItem::GroundItem(Ground const * ground) :
    #ifdef CLIENT_SIDE
    Item(ground->getName() + "_ground", ground->default_texture),
    #else
    Item(ground->getName() + "_ground", 0),
    #endif // CLIENT_SIDE
    ground(ground)
{

}

GroundItem::~GroundItem()
{
    //dtor
}

void GroundItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
    if (world.getGround(click_pos) == GameGrounds::DIRT)
    {
        stack.takeSome(1);

        #ifndef CLIENT_SIDE
        world.setGround(click_pos, ground);

        ECCPacket set;

        set << Networking::StoC::InventoryUpdate;
        set << InventoryUpdates::StoC::SetStack;
        set << 0;   //We set the first slot of the inventory (hand)
        set << stack.getInt();

        player.getClient().send(set);
        #endif
    }
}
