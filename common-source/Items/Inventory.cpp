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
{
    insertNewItemStack(ItemStack(GameBlocks::GOLD->getId(), 254));
}
#endif // CLIENT_SIDE

Inventory::~Inventory()
{
    //dtor
}

void Inventory::describe() const
{
    #ifndef CLIENT_SIDE
    //TEMP
    std::cout << "=== INVENTORY ===\n";
    for (const ItemStack& stack : contents)
    {
        if (stack.getAmount() != 0 && stack.getItemID() != 0)
            std::cout << server->getItemsRegister().getItemByID(stack.getItemID())->getName() << " x" << (int)stack.getAmount() << '\n'; //OOPH
    }
    std::cout <<  std::endl;
    #endif
}

bool Inventory::insertItemStack(ItemStack& stack)
{
    #ifndef CLIENT_SIDE
    std::cout << server->getItemsRegister().getItemByID(stack.getItemID())->getName() << " x" << (int)stack.getAmount() << '\n';
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
