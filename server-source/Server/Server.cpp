#include "Server.h"

#include <cassert>
#include <cstdlib>

#include <SFML/System/Sleep.hpp>

#include "../Version.h"
#include "../Utils/Utils.h"

#include "../World/World.h"

#include "../../common-source/Networking/NetworkingCodes.h"
#include "../Packets/HandshakePacket.h"
#include "../Packets/FullInventoryPacket.h"

//TEMP
#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Entities/GameEntities/TestEntity.h"

#include "../../server-source/World/EntitiesManager.h"
#include "../../server-source/World/Generator.h"
#include "../../server-source/World/Chunk.h"

#include "../../common-source/Utils/Log.h"


Server::Server(uint16_t client_port) :
      clients_manager(*this),
      receiver_thread(&Server::receiver, this),
      owner(sf::IpAddress::LocalHost, client_port),
      connection_open(false),
      blocks_manager(),
      grounds_manager(),
      world(std::make_unique<World>(*this))
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
    world->init();

    log(INFO, "\n");

    if (server_socket.bind(port) != sf::Socket::Done)
    {
        log(ERROR, "Could not bind server socket to port {}\n", port);
        return false;
    }
    log(INFO, "Bound server socket to port {} (UDP).\n", server_socket.getLocalPort());

    server_socket.setBlocking(true);

#ifdef SOLO
    clients_manager.addClient(owner);
#else
    connection_open = true;
#endif // SOLO

    running = true;
    receiver_thread.launch();

#ifdef SOLO
    unsigned int player_id = world->getEntityManager().getNextEntityId();

    Player* owner_player = new Player(*world, player_id, clients_manager.getClient(owner));
    clients_manager.getClient(owner).setPlayer(owner_player);

    ECCPacket handshake;
    handshake << Networking::StoC::FinalHandshake << Version::VERSION_SHORT;
    handshake << world->getGenerator()->getSeed();
    handshake << player_id;
    server_socket.send(handshake, owner.address, owner.port);

    FullInventoryPacket fip(owner_player->getInventory());
    server_socket.send(fip, owner.address, owner.port);

    world->getEntityManager().newEntity(owner_player);
#endif // SOLO

    for (int i = 0; i < 10; i++)
        world->getEntityManager().newEntity(new TestEntity(*world, world->getEntityManager().getNextEntityId()));

    //world.getEntityManager().newEntity(new Player(&world, world.getEntityManager().getNextEntityId(), clients_manager.getClient(owner)));
    return true;
}

void Server::run()
{
    server_clock.restart();
    float delta;

    sf::Clock crash_clock;

    while (running)
    {
        int ms = server_clock.getElapsedTime().asMilliseconds();

        if (ms > 50)
        {
            log(INFO, "Tick took too much time! Running {}ms behind.\n", (ms - 50));
        }
        else
            sf::sleep(sf::milliseconds(50 - ms));

        delta = server_clock.restart().asSeconds();

        if (timeout_checks.getElapsedTime().asSeconds() >= 5.f)
        {
            timeout_checks.restart();
            clients_manager.doTimeOuts(10.f);
        }

        processPacketQueue();

        //Update entities
        world->getEntityManager().updateAll(delta);
        world->updateTileEntities(delta);
        //Update all worlds

        /*
        if (crash_clock.getElapsedTime().asSeconds() >= 5.f)
            throw new std::logic_error("I AM A RETARDED SERVER");
        */
    }
}

void Server::close()
{
    receiver_thread.wait();

    ECCPacket server_stopping(Networking::StoC::Disconnect);
    clients_manager.sendToAll(server_stopping);

    server_socket.unbind();
}

void Server::receiver()
{
    while (running)
    {

        ECCPacket packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = server_socket.receive(packet, address, port);

        switch (status)
        {
            case sf::Socket::Done:
                if (packet.isCorrupted())
                {
                    log(ERROR, "Received a corrupted packet : {} bytes packet from {} : {}\n", packet.getDataSize(), address.toString(), port);
                }
                else if (packet.getDataSize() >= 2)
                {
                    IpAndPort iandp(address, port);

                    int code; packet >> code;

                    switch (code)
                    {
                        case Networking::CtoS::KeepAlive:
                            request_queue.pushRequest(Networking::CtoS::KeepAliveRequest{iandp});
                            break;
                        case Networking::CtoS::Disconnect:
                            request_queue.pushRequest(Networking::CtoS::DisconnectRequest{iandp});
                            break;
                        case Networking::CtoS::RequestConnection:
                            request_queue.pushRequest(Networking::CtoS::ConnectionRequest{iandp});
                            break;
                        case Networking::CtoS::RequestChunk:
                        {
                            sf::Vector2i pos;
                            packet >> pos.x;
                            packet >> pos.y;

                            request_queue.pushRequest(Networking::CtoS::ChunkRequest{iandp, pos});
                        }
                        break;
                        case Networking::CtoS::RequestEntityInfo:
                        {
                            unsigned int id; packet >> id;

                            request_queue.pushRequest(Networking::CtoS::EntityRequest{iandp, id});
                        }
                        break;
                        case Networking::CtoS::PlayerAction:
                        {
                            Networking::CtoS::PlayerActionRequest rq{iandp};

                            packet >> rq.type;
                            switch (rq.type)
                            {
                                case EntityActions::CtoS::Walk:
                                {
                                    packet >> rq.walk_mov_x >> rq.walk_mov_y;
                                    packet >> rq.walk_pos_x >> rq.walk_pos_y;

                                    if (!packet)
                                    {
                                        log(ERROR, "Could not read playerAction, packet too short\n");
                                        break;
                                    }
                                }
                                break;

                                case EntityActions::CtoS::UseItem:
                                {
                                    packet >> rq.item_pos_x >> rq.item_pos_y;
                                    packet >> rq.item_in_hand;

                                    if (!packet)
                                    {
                                        log(ERROR, "Could not read playerAction, packet too short\n");
                                        break;
                                    }
                                }
                                break;

                                case EntityActions::CtoS::BreakBlock:
                                {
                                    packet >> rq.break_block_pos.x >> rq.break_block_pos.y;

                                    if (!packet)
                                    {
                                        log(ERROR, "Could not read playerAction, packet too short\n");
                                        break;
                                    }
                                }
                                break;

                                case EntityActions::CtoS::SwapInventoryItem:
                                {
                                    packet >> rq.item_swap_pos;
                                    packet >> rq.hand_item;
                                    packet >> rq.slot_item;

                                    if (!packet)
                                        break;
                                }
                                break;
                            }
                            request_queue.pushRequest(std::move(rq));
                        }
                        break;

                        default:
                            log(ERROR, "Packet has unknown code\n");
                            break;
                    }
                }
                else
                    log(ERROR, "Packet is too small to be read\n");
                break;

            case sf::Socket::Disconnected: break;

            default:
                log(ERROR, "Packet has status {}\n", utils::statusToString(status));
                break;
        }
    }
}

void Server::processPacketQueue()
{
    using namespace Networking::CtoS;

    while (!request_queue.empty())
    {
        if (auto rq = request_queue.tryPop<KeepAliveRequest>())
        {
            if (!clients_manager.isConnected(rq->iandp))
            {
                ECCPacket disconnect(Networking::StoC::Disconnect);
                server_socket.send(disconnect, rq->iandp.address, rq->iandp.port);
            }
            else
                clients_manager.resetClientTimer(rq->iandp);
        }
        else if (auto rq = request_queue.tryPop<DisconnectRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

#ifdef SOLO
            if (rq->iandp.address == owner.address && rq->iandp.port == owner.port)
            {
                log(INFO, "Received disconnect message from owner, server will stop.\n");
                running = false;
                break;
            }
            else
#endif // SOLO
            {
                if (!clients_manager.isConnected(rq->iandp))
                    continue;

                log(INFO, "Disconnecting player.\n");

                Client& client = clients_manager.getClient(rq->iandp);
                if (client.hasPlayer())
                {
                    world->getEntityManager().removeEntity(client.getPlayer()->getId());
                    client.setPlayer(nullptr);
                }

                clients_manager.removeClient(rq->iandp);
                log(INFO, "Player disconnected.\n");
            }
        }
        else if (auto rq = request_queue.tryPop<ConnectionRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

            log(INFO, "Connection requested\n");

            if (!connection_open)
                continue;
            if (clients_manager.isConnected(rq->iandp))
                continue;

            log(INFO, "Connection accepted\n");
            {
                unsigned int player_id = world->getEntityManager().getNextEntityId();

                clients_manager.addClient(rq->iandp);
                Player* new_player = new Player(*world, player_id, clients_manager.getClient(rq->iandp));
                clients_manager.getClient(rq->iandp).setPlayer(new_player);

                //Handshake
                HandshakePacket handshake(world->getGenerator()->getSeed(), player_id);
                server_socket.send(handshake, rq->iandp.address, rq->iandp.port);

                //Send inventory packet
                FullInventoryPacket fip(new_player->getInventory());
                server_socket.send(fip, rq->iandp.address, rq->iandp.port);

                world->getEntityManager().newEntity(new_player);
            }
            log(INFO, "New player added!\n");
        }
        else if (auto rq = request_queue.tryPop<ChunkRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

            if (!clients_manager.isConnected(rq->iandp))
                continue;

            ECCPacket p = world->getChunk(rq->pos).getPacket();
            server_socket.send(p, rq->iandp.address, rq->iandp.port);
            world->getEntityManager().sendAddEntityFromAllEntitiesInChunk(rq->pos, clients_manager.getClient(rq->iandp));
        }
        else if (auto rq = request_queue.tryPop<EntityRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

            if (!clients_manager.isConnected(rq->iandp))
                continue;

            world->getEntityManager().sendAddEntityToClient(rq->id, clients_manager.getClient(rq->iandp));
        }
        else if (auto rq = request_queue.tryPop<PlayerActionRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

            if (!clients_manager.isConnected(rq->iandp))
                continue;

            Client& client = clients_manager.getClient(rq->iandp);

            if (!client.hasPlayer())
                continue;

            client.getPlayer()->handlePlayerActionRequest(rq.value());
        }
        else
        {
            log(ERROR, "Unknown CtoS packet request type; ignoring\n");
            request_queue.skip();
        }
    }
}

void Server::passReceiveOnce()
{
    //When we stop the server from outside the receiver, the receive function is blocking so it will wait for something to happen and block
    //To avoid that we send a packet to ourselves to pass the receive once, enough for the while loop of the thread to stop
    ECCPacket p(Networking::CtoS::KeepAlive);

    server_socket.send(p, sf::IpAddress::LocalHost, server_socket.getLocalPort());
}
