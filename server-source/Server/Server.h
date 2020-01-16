#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <atomic>

#include "../World/World.h"

#include "ClientsManager.h"

class Server
{
    friend class Client;
    friend class ClientsManager;

    public:
        Server(uint16_t client_port);   //Leave client port to 0 if this is a multiplayer server, i just don't want to have ifdef's everywhere
        ~Server();

        /**
         * Inits the server and the receiver threads
         * @param port : the port to use for the socket
         * @return True if initialized successfully
         */
        bool init(uint16_t port);

        /**
         * Runs the server. The server runs while "running" is true
         */
        void run();

        /**
         * Stops the sockets and threads
         */
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
        /**
         * Sends empty packet to self, needed to stop the receiver thread from outside
         */
        void passReceiveOnce(); //Comment inside definition

        IpAndPort owner;

        std::atomic<bool> connection_open;

        GameBlocks blocks_manager;
        GameGrounds grounds_manager;

        //Theres only one world for the moment
        World world;
};
