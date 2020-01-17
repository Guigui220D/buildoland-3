#include "Inventory.h"

#include <iostream>

#include "../Entities/Entity.h"

#ifndef CLIENT_SIDE
    #include "../../server-source/World/World.h"
    #include "../../server-source/Server/Server.h"
#endif

Inventory::Inventory(Entity const * owner) :
    owner(owner)
{

}

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
            std::cout << owner->getWorld()->getServer()->getItemsRegister().getItemByID(stack.getItemID())->getName() << " x" << (int)stack.getAmount() << '\n'; //OOPH
    }
    std::cout <<  std::endl;
    #endif
}
