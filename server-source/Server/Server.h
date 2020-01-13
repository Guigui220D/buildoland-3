#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <atomic>

#include "../Block/GameBlocks.h"
#include "../Ground/GameGrounds.h"
#include "../World/World.h"

#include "ClientsManager.h"

class Server
{
    friend class Client;

    public:
        Server(uint16_t client_port);   //Leave client port to 0 if this is a multiplayer server, i just don't want to have ifdef's everywhere
        ~Server();

        bool init(uint16_t port);

        void run();

        void close();

        inline GameBlocks& getBlocksManager() { return blocks_manager; }
        inline GameGrounds& getGroundsManager() { return grounds_manager; }

        inline ClientsManager& getClientsManager() { return clients_manager; }

        inline World& getWorld() { return world; }

    private:
        sf::UdpSocket server_socket;

        ClientsManager clients_manager;

        sf::Clock server_clock;
        sf::Clock timeout_checks;

        void receiver();
        sf::Thread receiver_thread;

        std::atomic<bool> running;

        IpAndPort owner;

        std::atomic<bool> connection_open;

        GameBlocks blocks_manager;
        GameGrounds grounds_manager;

        //Theres only one world for the moment
        World world;
};
