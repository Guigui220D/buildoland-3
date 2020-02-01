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
#include "../../common-source/Entities/GameEntities/TestEntity.h"

Server::Server(uint16_t client_port) :
    clients_manager(*this),
    receiver_thread(Server::receiver, this),
    owner(sf::IpAddress::LocalHost, client_port),
    connection_open(false),
    blocks_manager(),
    grounds_manager(),
    world(*this)
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
    std::srand(std::time(0));

    blocks_manager.initBlocks();
    grounds_manager.initGrounds();
    items_register.initItems(blocks_manager, grounds_manager);
    world.init();

    std::cout << std::endl;

    if (server_socket.bind(port) != sf::Socket::Done)
    {
        std::cerr << "Could not bind server socket to port " << port << std::endl;
        return false;
    }
    std::cout << "Bound server socket to port " << server_socket.getLocalPort() << " (UDP)." << std::endl;

    server_socket.setBlocking(true);

    #ifdef SOLO
        clients_manager.addClient(owner);
    #else
        connection_open = true;
    #endif // SOLO

    running = true;
    receiver_thread.launch();

    #ifdef SOLO
        unsigned int player_id = world.getEntityManager().getNextEntityId();

        Player* owner_player = new Player(&world, player_id, clients_manager.getClient(owner));
        clients_manager.getClient(owner).setPlayer(owner_player);

        sf::Packet handshake;
        handshake << Networking::StoC::FinalHandshake << Version::VERSION_SHORT;
        handshake << player_id;
        server_socket.send(handshake, owner.address, owner.port);

        world.getEntityManager().newEntity(owner_player);
    #endif // SOLO

    for (int i = 0; i < 10; i++)
        world.getEntityManager().newEntity(new TestEntity(world, world.getEntityManager().getNextEntityId()));

    //world.getEntityManager().newEntity(new Player(&world, world.getEntityManager().getNextEntityId(), clients_manager.getClient(owner)));
    return true;
}

void Server::run()
{
    server_clock.restart();
    float delta;

    while (running)
    {
        int ms = server_clock.getElapsedTime().asMilliseconds();

        if (ms > 50)
        {
            std::clog << "Tick took too much time! Running " << (ms - 50) << "ms behind." << std::endl;
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(50 - ms));

        delta = server_clock.restart().asSeconds();

        if (timeout_checks.getElapsedTime().asSeconds() >= 5.f)
        {
            timeout_checks.restart();
            clients_manager.doTimeOuts(10.f);
        }

        //Answer chunk requests
        clients_manager.clients_mutex.lock();
        for (auto i = clients_manager.getClientsBegin(); i != clients_manager.getClientsEnd(); i++)
        {
            if (i->second->hasRequestedChunks())
            {
                sf::Vector2i pos = i->second->getNextRequestedChunk();

                sf::Packet p = world.getChunk(pos).getPacket();
                server_socket.send(p, i->first.address, i->first.port);

                world.getEntityManager().sendAddEntityFromAllEntitiesInChunk(pos, *(i->second));
            }
        }
        clients_manager.clients_mutex.unlock();

        //Update entities
        world.getEntityManager().updateAll(delta);
        //Update all worlds
    }
}

void Server::close()
{
    receiver_thread.wait();

    sf::Packet server_stopping; server_stopping << Networking::StoC::Disconnect;
    clients_manager.sendToAll(server_stopping);

    server_socket.unbind();
}

void Server::receiver()
{
    //std::cout << "A" << std::endl;
    while (running)
    {
        //std::cout << "A" << std::endl;

        sf::Packet packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = server_socket.receive(packet, address, port);

        switch (status)
        {
        case sf::Socket::Done:
            //std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;
            if (packet.getDataSize() >= 2)
            {
                IpAndPort iandp(address, port);

                clients_manager.resetClientTimer(iandp);

                int code; packet >> code;

                switch (code)
                {
                case Networking::CtoS::KeepAlive: break;

                case Networking::CtoS::Disconnect:
                    #ifdef SOLO
                        if (address == owner.address && port == owner.port)
                        {
                            std::cout << "Received disconnect message from owner, server will stop." << std::endl;
                            running = false;
                            break;
                        }
                        else
                    #endif // SOLO
                    {
                        std::cout << "Disconnecting player." << std::endl;

                        if (!clients_manager.isConnected(iandp))
                            break;

                        Client& client = clients_manager.getClient(iandp);
                        if (client.hasPlayer())
                        {
                            world.getEntityManager().removeEntity(client.getPlayer()->getId());
                            client.setPlayer(nullptr);
                        }

                        clients_manager.removeClient(iandp);
                        std::cout << "Player disconnected." << std::endl;
                    }
                    break;
                case Networking::CtoS::RequestConnection:

                    std::clog << "Connection requested" << std::endl;

                    if (!connection_open)
                        break;
                    if (clients_manager.isConnected(iandp))
                        break;

                    std::clog << "Connection accepted" << std::endl;
                    {
                        unsigned int player_id = world.getEntityManager().getNextEntityId();

                        clients_manager.addClient(iandp);
                        Player* new_player = new Player(world, player_id, clients_manager.getClient(iandp));
                        clients_manager.getClient(iandp).setPlayer(new_player);

                        sf::Packet handshake;
                        handshake << Networking::StoC::FinalHandshake << Version::VERSION_SHORT;
                        handshake << player_id;

                        server_socket.send(handshake, address, port);

                        world.getEntityManager().newEntity(new_player);
                    }
                    std::clog << "New player added!" << std::endl;
                    break;
                case Networking::CtoS::RequestChunk:
                    {
                        if (!clients_manager.isConnected(iandp))
                            break;

                        sf::Vector2i pos;
                        packet >> pos.x;
                        packet >> pos.y;

                        //std::clog << "Adding chunk request" << std::endl;

                        clients_manager.getClient(iandp).addRequestedChunk(pos);
                    }
                    break;
                case Networking::CtoS::RequestEntityInfo:
                    {
                        if (!clients_manager.isConnected(iandp))
                            break;

                        unsigned int id; packet >> id;

                        world.getEntityManager().sendAddEntityToClient(id, clients_manager.getClient(iandp));
                    }
                    break;
                case Networking::CtoS::PlayerAction:
                    {
                        if (!clients_manager.isConnected(iandp))
                            break;

                        Client& client = clients_manager.getClient(iandp);

                        if (!client.hasPlayer())
                            break;

                        client.getPlayer()->takePlayerActionPacket(packet);
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

        case sf::Socket::Disconnected: break;

        default:
            std::cerr << "Packet has status " << utils::statusToString(status) << std::endl;
            //std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;
            break;
        }
    }
}

void Server::passReceiveOnce()
{
    //When we stop the server from outside the receiver, the receive function is blocking so it will wait for something to happen and block
    //To avoid that we send a packet to ourselves to pass the receive once, enough for the while loop of the thread to stop
    sf::Packet p; p << Networking::CtoS::KeepAlive;

    server_socket.send(p, sf::IpAddress::LocalHost, server_socket.getLocalPort());
}
