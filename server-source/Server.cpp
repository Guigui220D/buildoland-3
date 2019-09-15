#include "Server.h"

#include <iostream>
#include <assert.h>

Server::Server(uint16_t client_port) :
    receiver_thread(Server::receiver, this),
    client_port(client_port)
{
    #ifndef SOLO
        assert(!client_port)
    #endif // SOLO
}

Server::~Server()
{
    //dtor
}

bool Server::init(uint16_t port)
{
    if (server_socket.bind(port) != sf::Socket::Done)
    {
        std::cerr << "Could not bind server socket to port " << port << std::endl;
        return false;
    }
    std::cout << "Bound server socket to port " << server_socket.getLocalPort() << " (UDP)." << std::endl;

    server_socket.setBlocking(true);

    receiver_thread.launch();

    #ifdef SOLO
        sf::Packet handshake;
        handshake << 32;    //Test
        server_socket.send(handshake, sf::IpAddress::LocalHost, client_port);
    #endif // SOLO

    return true;
}

void Server::run()
{
    server_clock.restart();
    float delta;

    bool run = true;

    sf::Clock test;

    while (run)
    {
        while (server_clock.getElapsedTime().asSeconds() < 0.05f) {}
        delta = server_clock.restart().asSeconds();

        //Update everything
        //Update all worlds

        if (test.getElapsedTime().asSeconds() >= 3)
        {
            run = false;
        }

    }
}

void Server::close()
{
    receiver_thread.terminate();

    server_socket.unbind();
}

void Server::receiver()
{
    while (true)
    {
        sf::Packet packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = server_socket.receive(packet, address, port);

        switch (status)
        {
        case sf::Socket::Done:
            std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;
            break;
        case sf::Socket::NotReady:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was NOT READY." << std::endl;
            break;
        case sf::Socket::Partial:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was PARTIAL." << std::endl;
            break;
        case sf::Socket::Disconnected:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was DISCONNECTED." << std::endl;
            break;
        case sf::Socket::Error:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was ERROR." << std::endl;
            break;
        }
    }
}
