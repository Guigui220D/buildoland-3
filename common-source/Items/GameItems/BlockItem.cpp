#include "BlockItem.h"

#include "../ItemStack.h"

#include "../../Entities/GameEntities/Player.h"

#include "../../Blocks/GameBlocks.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/World/World.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Server/ClientsManager.h"
#endif // CLIENT_SIDE

BlockItem::BlockItem(Block const * block) :
    #ifdef CLIENT_SIDE
    Item(block->getName() + "_block", block->default_texture),
    #else
    Item(block->getName() + "_block", 0),
    #endif // CLIENT_SIDE
    block(block)
{
    //ctor
}

BlockItem::~BlockItem()
{
    //dtor
}

void BlockItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
    if (world.getBlock(click_pos) == GameBlocks::AIR)
    {
        stack.takeSome(1);

        #ifndef CLIENT_SIDE
        world.setBlock(click_pos, block);

        ECCPacket set;

        set << Networking::StoC::InventoryUpdate;
        set << InventoryUpdates::StoC::SetStack;
        set << 0;   //We set the first slot of the inventory (hand)
        set << stack.getInt();

        player.getClient().send(set);
        #endif
    }
}

