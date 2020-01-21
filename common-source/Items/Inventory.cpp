#include "Inventory.h"

#include <iostream>

#include "../Entities/Entity.h"

#ifndef CLIENT_SIDE
    #include "../../server-source/Server/Server.h"
#endif

#ifdef CLIENT_SIDE
Inventory::Inventory(Entity const * owner, GameState* game) :
    owner(owner),
    game(game)
{}
#else
Inventory::Inventory(Entity const * owner, Server* server) :
    owner(owner),
    server(server)
{}
#endif // CLIENT_SIDE

Inventory::~Inventory()
{
    //dtor
}

void Inventory::describe() const
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

bool Inventory::insertItemStack(ItemStack& stack)
{
    #ifndef CLIENT_SIDE
    std::cout << stack.getItem()->getName() << " x" << (int)stack.getAmount() << '\n';
    #endif

    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return true;
    return false;
}

void Inventory::insertNewItemStack(ItemStack stack)
{
    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return;
}
