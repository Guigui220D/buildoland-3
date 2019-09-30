#include "Server.h"

#include <iostream>
#include <assert.h>
#include <cstdlib>

#include <thread>
#include <chrono>

#include "../Version.h"
#include "../Utils/Utils.h"

#include "../../common-source/Networking/NetworkingCodes.h"

//TEMP
#include "../../common-source/Entities/GameEntities/Player.h"

Server::Server(uint16_t client_port) :
    clients_manager(this),
    receiver_thread(Server::receiver, this),
    owner(sf::IpAddress::LocalHost, client_port),
    connection_open(false),
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
    blocks_manager.initBlocks();
    grounds_manager.initGrounds();

    if (server_socket.bind(port) != sf::Socket::Done)
    {
        std::cerr << "Could not bind server socket to port " << port << std::endl;
        return false;
    }
    std::cout << "Bound server socket to port " << server_socket.getLocalPort() << " (UDP)." << std::endl;

    server_socket.setBlocking(true);

    #ifdef SOLO
        clients_manager.addClient(owner, new Player(&world, world.getEntityManager().getNextEntityId()));
    #else
        connection_open = true;
    #endif // SOLO

    receiver_thread.launch();

    #ifdef SOLO
        sf::Packet handshake;
        handshake << (unsigned short)Networking::StoC::FinalHandshake << Version::VERSION_SHORT;
        server_socket.send(handshake, owner.address, owner.port);
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
    int test_step = 0;

    run_mutex.lock();
    while (running)
    {
        run_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(50 - server_clock.getElapsedTime().asMilliseconds()));

        delta = server_clock.restart().asSeconds();

        //Answer chunk requests
        clients_manager.clients_mutex.lock();
        for (auto i = clients_manager.getClientsBegin(); i != clients_manager.getClientsEnd(); i++)
        {
            if (i->second->hasRequestedChunks())
            {
                sf::Vector2i pos = i->second->getNextRequestedChunk();

                sf::Packet p = world.getChunk(pos).getPacket();
                server_socket.send(p, i->first.address, i->first.port);
            }
        }
        clients_manager.clients_mutex.unlock();

        //Update entities
        world.getEntityManager().updateAll(delta);
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
                IpAndPort iandp(address, port);

                unsigned short code; packet >> code;

                switch (code)
                {
                case Networking::CtoS::Disconnect:
                    #ifdef SOLO
                        if (address == owner.address && port == owner.port)
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
                case Networking::CtoS::RequestConnection:
                    std::clog << "Connection requested" << std::endl;

                    if (!connection_open)
                        break;
                    if (clients_manager.isConnected(iandp))
                        break;

                    std::clog << "Connection accepted" << std::endl;

                    {
                        sf::Packet handshake;
                        handshake << (unsigned short)Networking::StoC::FinalHandshake << Version::VERSION_SHORT;
                        server_socket.send(handshake, address, port);
                    }

                    clients_manager.addClient(iandp, new Player(&world, world.getEntityManager().getNextEntityId()));

                    break;
                case Networking::CtoS::RequestChunk:
                    {
                        if (!clients_manager.isConnected(iandp))
                            break;

                        sf::Vector2i pos;
                        packet >> pos.x;
                        packet >> pos.y;

                        std::clog << "Adding chunk request" << std::endl;

                        clients_manager.getClient(iandp).addRequestedChunk(pos);
                    }
                    break;
                default:
                    std::cerr << "Packet has unknown code" << std::endl;
                    break;
                }



            }
            else
                std::cerr << "Packet is too small to be read" << std::endl;
            break;

        default:
            std::cerr << "Packet has status " << utils::statusToString(status) << std::endl;
            break;
        }
    }
}
