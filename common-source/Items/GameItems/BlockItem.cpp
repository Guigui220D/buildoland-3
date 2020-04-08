#include "BlockItem.h"

#include "../ItemStack.h"

#include "../../Entities/GameEntities/Player.h"

#include "../../Blocks/GameBlocks.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/World/World.h"
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/Server/ClientsManager.h"
    #include "../../../server-source/Packets/InventorySetPacket.h"
    #include "../../../server-source/Packets/SetTilePacket.h"
    #include "../../../server-source/Server/Server.h"
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
    #ifndef CLIENT_SIDE
    TileReference tr = world.getTile(click_pos);

    if (tr.getBlock() == GameBlocks::AIR)
    {
        //EWWW
        if (getBlock()->isSolid(BlockInfo(world, click_pos)))
        {
            for (auto i = world.getServer().getClientsManager().getClientsBegin(); i != world.getServer().getClientsManager().getClientsEnd(); i++)
            {
                if (i->second->hasPlayer())
                {
                    Player* player = i->second->getPlayer();
                    if (!player->canBlockBeHere(click_pos))
                        return;
                }
            }
        }

        stack.takeSome(1);

        tr.setBlock(block);

        InventorySetPacket set(0, stack.getInt());
        player.getClient().send(set);
    }
    else
    {
        SetTilePacket block_set(false, world.getBlockId(click_pos), click_pos);
        player.getClient().send(block_set);
    }
    #endif
}

