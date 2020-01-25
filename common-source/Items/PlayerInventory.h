#pragma once

#include "ItemStack.h"

#include <array>

class Player;

#ifdef CLIENT_SIDE
    #include <SFML/Network.hpp>
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

        bool insertItemStack(ItemStack& stack);
        void insertNewItemStack(ItemStack stack);

        void swapHands(int pos);

        #ifdef CLIENT_SIDE
        bool takeInventoryUpdatePacket(sf::Packet& packet);
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
