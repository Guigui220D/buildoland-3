#pragma once

#include "ItemStack.h"

#ifdef CLIENT_SIDE
#include "../Networking/ServerToClientRequests.h"
#endif

#include <array>

class Player;

#ifdef CLIENT_SIDE
class GameState;
#else
class Server;
#endif // CLIENT_SIDE

class PlayerInventory
{
    public:
        #ifdef CLIENT_SIDE
        PlayerInventory(const Player& owner, GameState& game);
        #else
        PlayerInventory(const Player& owner, Server& server);
        #endif // CLIENT_SIDE

        virtual ~PlayerInventory();

        std::array<ItemStack, 25> contents; //24 items and hand item

        void describe() const;

        bool insertItemStack(ItemStack& stack, bool drop_if_full = true);
        void insertNewItemStack(ItemStack stack, bool drop_if_full = true);

        void swapHands(int pos);
        void dropHand();

        #ifdef CLIENT_SIDE
        bool handleInventoryUpdateRequest(const Networking::StoC::InventoryUpdateRequest &rq);
        #endif // CLIENT_SIDE

    protected:

    private:
        const Player& owner;

        #ifdef CLIENT_SIDE
        GameState& game;
        #else
        Server& server;
        #endif // CLIENT_SIDE
};
