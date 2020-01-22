#include "PlayerInventory.h"

#include <iostream>

#include "../Entities/GameEntities/Player.h"
#include "../Networking/NetworkingCodes.h"

#ifndef CLIENT_SIDE
    #include "../../server-source/Server/Server.h"
#else
    #include "../../client-source/States/GameState.h"
    #include "../../client-source/Game.h"
#endif

#ifdef CLIENT_SIDE
PlayerInventory::PlayerInventory(const Player& owner, GameState& game) :
    owner(owner),
    game(game)
{}
#else
PlayerInventory::PlayerInventory(const Player& owner, Server& server) :
    owner(owner),
    server(server)
{}
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
    sf::Packet insert;

    insert << Networking::StoC::InventoryUpdate;
    insert << InventoryUpdates::StoC::AddStack;
    insert << stack.getInt();

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
    sf::Packet insert;

    insert << Networking::StoC::InventoryUpdate;
    insert << InventoryUpdates::StoC::AddStack;
    insert << stack.getInt();

    owner.getClient().send(insert);
    #endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return;
}

#ifdef CLIENT_SIDE
bool PlayerInventory::takeInventoryUpdatePacket(sf::Packet& packet)
{
    int type; packet >> type;

    if (!packet)
    {
        std::cerr << "Could not read inventory update, packet too short to get type." << std::endl;
        return false;
    }


    switch (type)
    {
    case InventoryUpdates::StoC::AddStack:
        {
            uint32_t itemstack; packet >> itemstack;
            if (packet)
            {
                ItemStack stack(itemstack, game.getGame().getItemsRegister());
                std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';
                insertItemStack(stack);
            }
        }
        describe();
        return true;
    case InventoryUpdates::StoC::SetStack:
        {
            unsigned int pos; packet >> pos;
            uint32_t itemstack; packet >> itemstack;
            if (packet)
            {
                if (pos >= contents.size())
                    return false;

                ItemStack stack(itemstack, game.getGame().getItemsRegister());
                std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';

                contents.at(pos).swap(stack);
            }
        }
        describe();
        return true;
    default:
        std::cerr << "Could not read inventory update, unknown type." << std::endl;
        return false;
    }
}
#endif // CLIENT_SIDE
