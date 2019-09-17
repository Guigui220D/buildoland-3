#pragma once

#include <SFML/Network.hpp>
#include <vector>

#include "Block/GameBlocks.h"
#include "Ground/GameGrounds.h"

#include "World/World.h"

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

        sf::Clock server_clock;

        void receiver();
        sf::Thread receiver_thread;

        bool running;
        sf::Mutex run_mutex;

        uint16_t client_port;   //Only for local servers

        GameBlocks blocksManager;
        GameGrounds groundsManager;

        //Theres only one world for the moment
        World world;

        //TEST
        std::vector<sf::Vector2i> requested_chunks;
        sf::Mutex requested_chunks_mutex;
};
