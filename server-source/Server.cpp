#include "Server.h"

#include <iostream>

Server::Server() :
    receiver_thread(Server::receiver, this)
{
    //ctor
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

    return true;
}

void Server::run()
{
    server_clock.restart();
    float delta;

    bool run = true;

    sf::Clock test;

    receiver_thread.launch();

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
