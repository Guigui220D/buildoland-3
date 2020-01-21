#include "PlayerInventory.h"

#include <iostream>

#include "../Entities/GameEntities/Player.h"

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
{
    insertNewItemStack(ItemStack(GameBlocks::GOLD->getDefaultItem(), 254));
    //insertNewItemStack(ItemStack(ItemsRegister::BALL, 3));
}
#endif // CLIENT_SIDE

PlayerInventory::~PlayerInventory()
{
    //dtor
}

void PlayerInventory::describe() const
{
    #ifndef CLIENT_SIDE
    //TEMP
    std::cout << "=== INVENTORY ===\n";
    for (const ItemStack& stack : contents)
    {
        if (stack.getAmount() != 0 && stack.getItem()->getId() != 0)
            std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n'; //OOPH
    }
    std::cout <<  std::endl;
    #endif
}

bool PlayerInventory::insertItemStack(ItemStack& stack)
{
    #ifndef CLIENT_SIDE
    std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';
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
