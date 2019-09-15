#pragma once

#include <SFML/Network.hpp>

class Server
{
    public:
        Server();
        ~Server();

        bool init(uint16_t port);

        void run();

        void close();

    private:
        sf::UdpSocket server_socket;

        sf::Clock server_clock;

        void receiver();
        sf::Thread receiver_thread;
};
