#include "PlayerInventory.h"

#include <iostream>

#include "../Entities/GameEntities/Player.h"
#include "../Networking/NetworkingCodes.h"

#ifndef CLIENT_SIDE
    #include "../../server-source/Server/Server.h"
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
        if (stack.getAmount() != 0 && stack.getItem()->getId() != 0)
            std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n'; //OOPH
    }
    std::cout <<  std::endl;
}

bool PlayerInventory::insertItemStack(ItemStack& stack)
{
    #ifndef CLIENT_SIDE
    sf::Packet insert;

    insert << (unsigned short)Networking::StoC::InventoryUpdate;
    insert << (unsigned short)InventoryUpdates::StoC::AddStack;
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
    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return;
}

#ifdef CLIENT_SIDE
bool PlayerInventory::takeInventoryUpdatePacket(sf::Packet& packet)
{
    unsigned short type; packet >> type;

    if (!packet)
    {
        std::cerr << "Could not read inventory update, packet too short to get type." << std::endl;
        return false;
    }


    switch (type)
    {
    case InventoryUpdates::StoC::AddStack:
        std::cout << "ADD STACK" << std::endl;
        return true;
    default:
        std::cerr << "Could not read inventory update, unknown type." << std::endl;
        return false;
    }
}
#endif // CLIENT_SIDE
