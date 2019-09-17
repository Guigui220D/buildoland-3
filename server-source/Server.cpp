#include "Server.h"

#include <iostream>
#include <assert.h>
#include <cstdlib>

#include "Version.h"
#include "Utils/Utils.h"

#include "../common/Networking/ClientToServerCodes.h"
#include "../common/Networking/ServerToClientCodes.h"

Server::Server(uint16_t client_port) :
    receiver_thread(Server::receiver, this),
    client_port(client_port),
    world(this)
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
    blocksManager.initBlocks();
    groundsManager.initGrounds();

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
        handshake << (unsigned short)Networking::StoC::SoloHandshake << Version::VERSION_SHORT;
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

    //THIS IS A TEST
    //if (false)
    {
        sf::Packet p;
        world.getChunk(sf::Vector2i(0, 0)).getPacket(p);
        server_socket.send(p, sf::IpAddress::LocalHost, client_port);
    }

    run_mutex.lock();
    while (running)
    {
        run_mutex.unlock();

        while (server_clock.getElapsedTime().asSeconds() < 0.05f) {}
        delta = server_clock.restart().asSeconds();


        requested_chunks_mutex.lock();
        for (sf::Vector2i& pos : requested_chunks)
        {
            sf::Packet p;
            world.getChunk(pos).getPacket(p);
            server_socket.send(p, sf::IpAddress::LocalHost, client_port);
        }
        requested_chunks.clear();
        requested_chunks_mutex.unlock();
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
            if (packet.getDataSize() >= 2)
            {
                unsigned short code; packet >> code;

                switch (code)
                {
                case Networking::CtoS::Disconnect:
                    #ifdef SOLO
                        if (address == sf::IpAddress::LocalHost && port == client_port)
                        {
                            std::cout << "Received disconnect message from owner, server will stop." << std::endl;
                            stop = true;
                            run_mutex.lock();
                            running = false;
                            run_mutex.unlock();
                            break;
                        }
                    #endif // SOLO
                    break;
                case Networking::CtoS::RequestChunk:
                    {
                        sf::Vector2i pos;
                        packet >> pos.x;
                        packet >> pos.y;

                        requested_chunks_mutex.lock();
                        requested_chunks.push_back(pos);
                        requested_chunks_mutex.unlock();
                    }
                default:
                    std::cerr << "Packet has unknown code" << std::endl;
                    break;
                }
            }
            else
                std::cerr << "Packet is too small to be read" << std::endl;
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

        default:
            std::cerr << "Packet has status " << utils::statusToString(status) << std::endl;
            break;
        }
    }
}
