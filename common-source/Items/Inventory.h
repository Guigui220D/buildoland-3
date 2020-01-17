#pragma once

#include "ItemStack.h"

#include <array>

class Entity;

#ifdef CLIENT_SIDE
class GameState;
#else
class Server;
#endif // CLIENT_SIDE

class Inventory
{
    public:
        #ifdef CLIENT_SIDE
        Inventory(Entity const * owner, GameState* game);
        #else
        Inventory(Entity const * owner, Server* server);
        #endif // CLIENT_SIDE

        virtual ~Inventory();

        std::array<ItemStack, 24> contents;

        void describe() const;

    protected:

    private:
        Entity const * owner;

        #ifdef CLIENT_SIDE
        GameState* game;
        #else
        Server* server;
        #endif // CLIENT_SIDE
};
