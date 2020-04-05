#include "PlayerInventory.h"

#include <iostream>

#include "../Entities/GameEntities/Player.h"
#include "../Networking/NetworkingCodes.h"

#ifndef CLIENT_SIDE
    #include "../../server-source/Server/Server.h"
    #include "../../server-source/Packets/InventoryAddPacket.h"
#else
    #include "../../client-source/States/GameState.h"
    #include "../../client-source/Game.h"

    #include "../../client-source/Packets/InventorySwapPacket.h"
#endif

#ifdef CLIENT_SIDE
PlayerInventory::PlayerInventory(const Player& owner, GameState& game) :
    owner(owner),
    game(game)
{
    ItemStack a(ItemsRegister::SHOVEL, 1);
    contents.at(0).swap(a);
}
#else
PlayerInventory::PlayerInventory(const Player& owner, Server& server) :
    owner(owner),
    server(server)
{
    ItemStack a(ItemsRegister::SHOVEL, 1);
    contents.at(0).swap(a);
}
#endif // CLIENT_SIDE

PlayerInventory::~PlayerInventory()
{
    //dtor
}

void PlayerInventory::describe() const
{
    //TEMP
    std::cout << "=== INVENTORY ===\n";
    for (const ItemStack& stack : contents)
    {
        if (stack)
            std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';
    }
    std::cout <<  std::endl;
}

bool PlayerInventory::insertItemStack(ItemStack& stack)
{
    #ifndef CLIENT_SIDE
    InventoryAddPacket insert(stack.getInt());
    owner.getClient().send(insert);
    #endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return true;
    return false;
}

void PlayerInventory::insertNewItemStack(ItemStack stack)
{
    #ifndef CLIENT_SIDE
    InventoryAddPacket insert(stack.getInt());
    owner.getClient().send(insert);
    #endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return;
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

#ifdef CLIENT_SIDE
bool PlayerInventory::handleInventoryUpdateRequest(const Networking::StoC::InventoryUpdateRequest& rq)
{
    switch (rq.type)
    {
    case InventoryUpdates::StoC::AddStack:
        {
                ItemStack stack(rq.stack_add, game.getGame().getItemsRegister());
                //std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';
                insertItemStack(stack);
        }
        //describe();
        return true;
    case InventoryUpdates::StoC::SetStack:
        {
            if (rq.pos >= contents.size())
                return false;

            ItemStack stack(rq.stack_set, game.getGame().getItemsRegister());
            //std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';

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
        std::cerr << "Could not read inventory update, unknown type." << std::endl;
        return false;
    }
}
#endif // CLIENT_SIDE
