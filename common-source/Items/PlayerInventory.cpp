#include "playerInventory.h"

#include "../Entities/GameEntities/Player.h"
#include "../Networking/NetworkingCodes.h"

#include "../../common-source/Items/ItemsRegister.h"

#ifndef CLIENT_SIDE
#include "../../server-source/Server/Server.h"
#include "../../server-source/World/World.h"
#include "../../server-source/World/EntitiesManager.h"
#include "../../common-source/Entities/GameEntities/DroppedItemEntity.h"
#include "../../server-source/Packets/InventoryAddPacket.h"

#include "../Blocks/Block.h"
#else
#include "../../client-source/States/GameState.h"
#include "../../client-source/Game.h"

#include "../../client-source/Packets/InventorySwapPacket.h"
#include "../../client-source/Packets/InventoryDropPacket.h"
#endif

#include "../../common-source/Utils/Log.h"

#ifdef CLIENT_SIDE
PlayerInventory::PlayerInventory(const Player& owner, GameState& game) :
      owner(owner),
      game(game)
{}
#else
PlayerInventory::PlayerInventory(const Player& owner, Server& server) :
      owner(owner),
      server(server)
{
    { ItemStack a(ItemsRegister::SHOVEL, 1); contents.at(0).swap(a); }
    { ItemStack a(ItemsRegister::EMPTY_BUCKET, 1); contents.at(1).swap(a); }
}
#endif // CLIENT_SIDE

PlayerInventory::~PlayerInventory()
{
    //dtor
}

void PlayerInventory::describe() const
{
    //TEMP
    log(INFO, "=== INVENTORY ===\n");
    for (const ItemStack& stack : contents)
    {
        if (stack)
            log(INFO, "{} x {}\n", stack.getItem()->getName(),(int)stack.getAmount());
    }
    log(INFO, "\n");
}

bool PlayerInventory::insertItemStack(ItemStack& stack, bool drop_if_full)
{
#ifndef CLIENT_SIDE
    InventoryAddPacket insert(stack.getInt());
    owner.getClient().send(insert);
#endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return true;

// inventory is full; create a DroppedItem instead
#ifndef CLIENT_SIDE
    if (drop_if_full)
    {
        DroppedItemEntity* ent = new DroppedItemEntity(server.getWorld(), server.getWorld().getEntityManager().getNextEntityId(), owner.getPosition());
        ent->setItemStack(stack);
        server.getWorld().getEntityManager().newEntity(ent);
    }
#endif
    return false;
}

void PlayerInventory::insertNewItemStack(ItemStack stack, bool drop_if_full)
{
#ifndef CLIENT_SIDE
    InventoryAddPacket insert(stack.getInt());
    owner.getClient().send(insert);
#endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return;

// inventory is full; create a DroppedItem instead
#ifndef CLIENT_SIDE
    if (drop_if_full)
    {
    DroppedItemEntity* ent = new DroppedItemEntity(server.getWorld(), server.getWorld().getEntityManager().getNextEntityId(), owner.getPosition());
        ent->setItemStack(stack);
        server.getWorld().getEntityManager().newEntity(ent);
    }
#endif
}

void PlayerInventory::swapHands(int pos)
{
    if (pos <= 0 || pos >= 25)
        return;

#ifdef CLIENT_SIDE
    InventorySwapPacket swap(pos, contents.at(0).getInt(), contents.at(pos).getInt());

    game.sendToServer(swap);
#endif

    if (!contents.at(pos).add(contents.at(0)))
        contents.at(0).swap(contents.at(pos));
}

void PlayerInventory::dropHand()
{
    if (!contents.at(0))
        return;

#ifdef CLIENT_SIDE
    InventoryDropPacket drop(contents.at(0).getInt());

    game.sendToServer(drop);

    //    log(INFO, "Dropping stack {}\n", dropped_stack.getInt());
#endif

    ItemStack dropped_stack = contents.at(0);
    contents.at(0).reset();

#ifndef CLIENT_SIDE
    DroppedItemEntity* ent = new DroppedItemEntity(server.getWorld(), server.getWorld().getEntityManager().getNextEntityId(), owner.getPosition());
    ent->setItemStack(dropped_stack);
    server.getWorld().getEntityManager().newEntity(ent);
#endif
}

#ifdef CLIENT_SIDE
bool PlayerInventory::handleInventoryUpdateRequest(const Networking::StoC::InventoryUpdateRequest& rq)
{
    switch (rq.type)
    {
        case InventoryUpdates::StoC::AddStack:
        {
            ItemStack stack(rq.stack_add, game.getGame().getItemsRegister());
            insertItemStack(stack);
        }
            //describe();
            return true;
        case InventoryUpdates::StoC::SetStack:
        {
            if (rq.pos >= contents.size())
                return false;

            ItemStack stack(rq.stack_set, game.getGame().getItemsRegister());

            contents.at(rq.pos).swap(stack);
        }
            //describe();
            return true;

        case InventoryUpdates::StoC::SetInventory:
        {
            for (int i = 0; i < 25; i++)
            {
                ItemStack itemstack(rq.stack_list[i], game.getGame().getItemsRegister());

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
