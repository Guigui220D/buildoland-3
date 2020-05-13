#pragma once

#include <array>
#include <optional>

#include "ItemStack.h"

class World;

template <size_t INV_SIZE>
class Inventory
{
    public:
        Inventory(World& world);
        virtual ~Inventory();

        inline World& getWorld() { return world; }

        inline size_t getSize() const { return INV_SIZE; }
        ItemStack& getStack(size_t position);

        bool insertItemStack(ItemStack& stack, std::optional<sf::Vector2f> drop_place);

        void describe();

        //nlohmann::json* serializeToJson() const;
        //void deserialize(nlohmann::json json);

    protected:
        std::array<ItemStack, INV_SIZE> contents;

    private:
        World& world;
};

//Aaaaah template class c:

#include <algorithm>
#include <exception>

#ifdef CLIENT_SIDE
    #include "../../client-source/World/World.h"
#else
    #include "../../server-source/World/World.h"

    #include "../../server-source/World/EntitiesManager.h"
    #include "../../common-source/Entities/GameEntities/DroppedItemEntity.h"
#endif // CLIENT_SIDE

template <size_t INV_SIZE>
Inventory<INV_SIZE>::Inventory(World& world) :
    world(world)
{
    for (size_t i = 0; i < INV_SIZE; i++)
    {
        ItemStack is;
        contents.at(i).swap(is);
    }
}

template <size_t INV_SIZE>
Inventory<INV_SIZE>::~Inventory()
{
    //dtor
}

template <size_t INV_SIZE>
ItemStack& Inventory<INV_SIZE>::getStack(size_t position)
{
    return contents.at(position);   //No bounds checking, .at can throw anyway
}

template <size_t INV_SIZE>
bool Inventory<INV_SIZE>::insertItemStack(ItemStack& stack, std::optional<sf::Vector2f> drop_place)
{
    //Try to fill slots that have the same item type
    for (ItemStack& istack : contents)
        if (istack.getItem() == stack.getItem())
            if (istack.add(stack))
                return true;

    //Fill any slot that can accept the stack
    for (ItemStack& istack : contents)
        if (istack.add(stack))
            return true;

    //Inventory is full; create a DroppedItem instead
#ifndef CLIENT_SIDE
    if (drop_place)
    {
        DroppedItemEntity* ent = new DroppedItemEntity(world, world.getEntityManager().getNextEntityId());
        ent->setPosition(*drop_place);
        ent->setItemStack(stack);
        world.getEntityManager().newEntity(ent);
    }
#endif
    return false;
}
