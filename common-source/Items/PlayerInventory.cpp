#include "PlayerInventory.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/States/GameState.h"

    #include "../../client-source/Packets/InventorySwapPacket.h"
    #include "../../client-source/Packets/InventoryDropPacket.h"

    #include "../Networking/StoC_InventoryUpdateCodes.h"
#else
    #include "../../server-source/Packets/InventoryAddPacket.h"

    #include "../../server-source/Server/Server.h"
    #include "../../server-source/World/World.h"
    #include "../../server-source/World/EntitiesManager.h"

    #include "../Entities/GameEntities/DroppedItemEntity.h"

    #include "ItemsRegister.h"

    #include "../Blocks/GameBlocks.h"
    #include "../Blocks/Block.h"
#endif // CLIENT_SIDE

#include "../Entities/GameEntities/Player.h"

PlayerInventory::PlayerInventory(const Player& owner) :
    Inventory(owner.getWorld()),
    owner(owner)
{
#ifndef CLIENT_SIDE
    { ItemStack a(ItemsRegister::SHOVEL, 1);                insertItemStack(a, {}); }
    { ItemStack a(ItemsRegister::EMPTY_BUCKET, 1);          insertItemStack(a, {}); }
    { ItemStack a(GameBlocks::TEST->getDefaultItem(), 10);  insertItemStack(a, {}); }
#endif // CLIENT_SIDE
    preparing = false;
}

/*
PlayerInventory::~PlayerInventory()
{
    //dtor
}
*/

void PlayerInventory::swapHands(int pos)
{
    if (pos <= 0 || pos >= 25)
        return;

#ifdef CLIENT_SIDE
    InventorySwapPacket swap(pos, getHand().getInt(), contents.at(pos).getInt());

    getWorld().getState().sendToServer(swap);
#endif

    if (!getStack(pos).add(getHand()))
        getHand().swap(getStack(pos));
}

void PlayerInventory::dropHand()
{
    if (!getHand())
        return;

#ifdef CLIENT_SIDE
    InventoryDropPacket drop(getHand().getInt());

    getWorld().getState().sendToServer(drop);
#endif

    ItemStack dropped_stack;
    getHand().swap(dropped_stack);

#ifndef CLIENT_SIDE
    DroppedItemEntity* ent = new DroppedItemEntity(getWorld(), getWorld().getEntityManager().getNextEntityId());
    ent->setPosition(owner.getPosition());
    ent->setItemStack(dropped_stack);
    getWorld().getEntityManager().newEntity(ent);
#endif
}

#ifdef CLIENT_SIDE
bool PlayerInventory::handleInventoryUpdateRequest(const Networking::StoC::InventoryUpdateRequest& rq)
{
    switch (rq.type)
    {
        case InventoryUpdates::StoC::AddStack:
        {
            ItemStack stack(rq.stack_add, getWorld().getGame().getItemsRegister());
            insertItemStack(stack, owner.getPosition());
        }
            //describe();
            return true;
        case InventoryUpdates::StoC::SetStack:
        {
            if (rq.pos >= contents.size())
                return false;

            ItemStack stack(rq.stack_set, getWorld().getGame().getItemsRegister());

            contents.at(rq.pos).swap(stack);
        }
            //describe();
            return true;

        case InventoryUpdates::StoC::SetInventory:
        {
            for (int i = 0; i < 25; i++)
            {
                ItemStack itemstack(rq.stack_list[i], getWorld().getGame().getItemsRegister());

                contents.at(i).swap(itemstack);
            }
        }
            return true;

        default:
            log(ERROR, "Could not read inventory update, unknown type.\n");
            return false;
    }
}
#endif // CLIENT_SIDE

void PlayerInventory::onInsert(const ItemStack& stack)
{
#ifndef CLIENT_SIDE
    InventoryAddPacket insert(stack.getInt());
    if (!preparing)
        owner.getClient().send(insert);
#endif
}
