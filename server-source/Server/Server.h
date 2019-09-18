#pragma once

#include <SFML/Network.hpp>
#include <vector>

#include "../Block/GameBlocks.h"
#include "../Ground/GameGrounds.h"
#include "../World/World.h"

#include "ClientsManager.h"

class Server
{
    public:
        Server(uint16_t client_port);   //Leave client port to 0 if this is a multiplayer server, i just don't want to have ifdef's everywhere
        ~Server();

        bool init(uint16_t port);

        void run();

        void close();

        inline GameBlocks& getBlocksManager() { return blocksManager; }
        inline GameGrounds& getGroundsManager() { return groundsManager; }

    private:
        sf::UdpSocket server_socket;

        ClientsManager clients_manager;

        sf::Clock server_clock;

        void receiver();
        sf::Thread receiver_thread;

        bool running;
        sf::Mutex run_mutex;

        IpAndPort owner;

        bool connection_open;

        GameBlocks blocksManager;
        GameGrounds groundsManager;

        //Theres only one world for the moment
        World world;
        //TEST
        std::vector<sf::Vector2i> requested_chunks;
        sf::Mutex requested_chunks_mutex;
};
