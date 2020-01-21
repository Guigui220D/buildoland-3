#pragma once

#include "ItemStack.h"

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

        std::array<ItemStack, 24> contents;

        void describe() const;

        bool insertItemStack(ItemStack& stack);
        void insertNewItemStack(ItemStack stack);

    protected:

    private:
        const Player& owner;

        #ifdef CLIENT_SIDE
        GameState& game;
        #else
        Server& server;
        #endif // CLIENT_SIDE
};
