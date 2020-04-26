#pragma once

#include <vector>
#include <atomic>
#include <unordered_map>

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Thread.hpp>

#include "../../common-source/Networking/ClientToServerRequests.h"
#include "../../common-source/Networking/NetworkRequestQueue.h"
#include "../../common-source/Items/ItemsRegister.h"

#include "ClientsManager.h"

class World;

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

        inline ItemsRegister& getItemsRegister() { return items_register; }

        inline ClientsManager& getClientsManager() { return clients_manager; }

        inline World& getWorld() { return *world; }

    private:
        struct Command
        {
            std::function<void(Client& client, const std::string& arguments)> callback;
            bool admin_command;
        };
        std::unordered_map<std::string, Command> commands;
        void initCommands();
        void handleCommand(Client &client, const std::string& command, const std::string &args);

        sf::UdpSocket server_socket;

        ClientsManager clients_manager;

        sf::Clock server_clock;
        sf::Clock timeout_checks;

        void receiver();
        void processPacketQueue();
        sf::Thread receiver_thread;
        Networking::CtoS::CtoSRequestQueue request_queue;

        std::atomic<bool> running;
        /**
         * Sends empty packet to self, needed to stop the receiver thread from outside
         */
        void passReceiveOnce(); //Comment inside definition

        IpAndPort owner;

        bool connection_open;

        GameBlocks blocks_manager;
        GameGrounds grounds_manager;
        ItemsRegister items_register;

        //Theres only one world for the moment
        std::unique_ptr<World> world;
};
