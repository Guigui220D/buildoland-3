#include "Server.h"

#include <iostream>
#include <assert.h>
#include <cstdlib>

#include "Version.h"

Server::Server(uint16_t client_port) :
    receiver_thread(Server::receiver, this),
    client_port(client_port)
{
    #ifndef SOLO
        assert(!client_port);
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
        handshake << 0 << Version::VERSION_SHORT;
        server_socket.send(handshake, sf::IpAddress::LocalHost, client_port);

    #endif // SOLO

    return true;
}

void Server::run()
{
    server_clock.restart();
    float delta;

    run_mutex.lock();
    running = true;
    run_mutex.unlock();

    sf::Clock test;

    //Test
    sf::Packet chunk;
    chunk << uint16_t(1);
    chunk << 0 << 0;
    for (int i = 0; i < 16 * 16; i++)
    {
        chunk << (uint16_t)(std::rand() % 2 ? std::rand() % 5 : 0);
    }
    for (int i = 0; i < 16 * 16; i++)
    {
        chunk << (uint16_t)(std::rand() % 5);
    }
    server_socket.send(chunk, sf::IpAddress::LocalHost, client_port);

    run_mutex.lock();
    while (running)
    {
        run_mutex.unlock();

        while (server_clock.getElapsedTime().asSeconds() < 0.05f) {}
        delta = server_clock.restart().asSeconds();

        //Update everything
        //Update all worlds
        run_mutex.lock();
    }
    run_mutex.unlock();
}

void Server::close()
{
    receiver_thread.wait();

    server_socket.unbind();
}

void Server::receiver()
{
    bool stop = false;
    while (true)
    {
        if (stop)
            break;
        sf::Packet packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = server_socket.receive(packet, address, port);

        switch (status)
        {
        case sf::Socket::Done:
            std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;
            if (packet.getDataSize() == 4)
            {
                int i;
                packet >> i;
                if (i == 0)
                {
                    std::cout << "Received disconnect message, server will stop." << std::endl;
                    stop = true;
                    run_mutex.lock();
                    running = false;
                    run_mutex.unlock();
                }
            }
            break;
        case sf::Socket::NotReady:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was NOT READY." << std::endl;
            break;
        case sf::Socket::Partial:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was PARTIAL." << std::endl;
            break;
        case sf::Socket::Disconnected:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was DISCONNECTED." << std::endl;
            #ifdef SOLO
                std::cout << "Received disconnect status, server will stop." << std::endl;
                stop = true;
                run_mutex.lock();
                running = false;
                run_mutex.unlock();
            #endif // SOLO
            break;
        case sf::Socket::Error:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was ERROR." << std::endl;
            break;
        }
    }
}
