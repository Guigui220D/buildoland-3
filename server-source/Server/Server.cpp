#include "Server.h"

#include <cassert>
#include <cstdlib>

#include <SFML/System/Sleep.hpp>

#include "../Version.h"
#include "../Utils/Utils.h"

#include "../World/World.h"
#include "../World/EntitiesManager.h"
#include "../World/Chunk.h"
#include "../World/Generator.h"

#include "../../common-source/Networking/NetworkingCodes.h"
#include "../Packets/HandshakePacket.h"
#include "../Packets/FullInventoryPacket.h"

#include "../../common-source/Entities/GameEntities/Player.h"

#include "../../common-source/Utils/Log.h"
#include "../../common-source/Utils/UsernameCheck.h"

Server::Server(uint16_t client_port) :
      clients_manager(*this),
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
    log(INFO, "Destructing server! Bye bye :)\n");
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
    clients_manager.addClient(owner, "Me");
#else
    connection_open = true;
#endif // SOLO

    running = true;
    receiver_thread = std::thread(&Server::receiver, this);

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

    initCommands();

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

        world->updateChunks();

        //Update entities
        world->getEntityManager().updateAll(delta);
        world->updateTileEntities(delta);
        //Update all worlds

        /*
        if (crash_clock.getElapsedTime().asSeconds() >= 120.f)
            stop();
            */
    }
}

void Server::close()
{
    running = false;
    passReceiveOnce();

    if (receiver_thread.joinable())
        receiver_thread.join();

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
                        {
                            std::string nickname;
                            packet >> nickname;
                            request_queue.pushRequest(Networking::CtoS::ConnectionRequest{iandp, nickname});
                        }
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
                                case EntityActions::CtoS::DropInventoryItem:
                                {
                                    packet >> rq.hand_item;

                                    if (!packet)
                                        break;
                                }
                                case EntityActions::CtoS::EntityLeftClick:
                                case EntityActions::CtoS::EntityRightClick:
                                {
                                    packet >> rq.entity_id;

                                    if (!packet)
                                        break;
                                }
                                break;
                            }
                            request_queue.pushRequest(std::move(rq));
                        }
                        break;
                        case Networking::CtoS::SendMessage:
                        {
                            sf::String msg; packet >> msg;

                            request_queue.pushRequest(Networking::CtoS::SendMessageRequest{iandp, msg});
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
                stop();
                break;
            }
            else
#endif // SOLO
            {
                if (!clients_manager.isConnected(rq->iandp))
                    continue;

                log(INFO, "Disconnecting player.\n");

                Client& client = clients_manager.getClient(rq->iandp);
                std::string nickname = client.getNickname();
                if (client.hasPlayer())
                {
                    world->getEntityManager().removeEntity(client.getPlayer()->getId());
                    client.setPlayer(nullptr);
                }

                clients_manager.removeClient(rq->iandp);
                log(INFO, "Player disconnected.\n");

                ECCPacket player_left_packet(Networking::StoC::PlayerDisconnected);
                player_left_packet << nickname;
                for (auto it = clients_manager.getClientsBegin(); it != clients_manager.getClientsEnd(); ++it)
                {
                    it->second->send(player_left_packet);
                }
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
            if (!UsernameCheck::checkUsername(rq->nickname))
            {
                ECCPacket packet(Networking::StoC::ConnectionRefused);
                packet << (uint8_t)Networking::StoC::ConnectionRefusalReason::InvalidNickname;
                server_socket.send(packet, rq->iandp.address, rq->iandp.port);
                log(INFO, "Invalid username\n");
                continue;
            }
            for (auto it = clients_manager.getClientsBegin(); it != clients_manager.getClientsEnd(); ++it)
            {
                if (rq->nickname == it->second->getNickname())
                {
                    ECCPacket packet(Networking::StoC::ConnectionRefused);
                    packet << (uint8_t)Networking::StoC::ConnectionRefusalReason::NicknameAlreadyChosen;
                    server_socket.send(packet, rq->iandp.address, rq->iandp.port);
                    log(INFO, "Username already taken\n");
                    goto loop;
                }
            }

            log(INFO, "Connection accepted\n");
            {
                unsigned int player_id = world->getEntityManager().getNextEntityId();

                ECCPacket new_player_packet(Networking::StoC::PlayerConnected);
                new_player_packet << rq->nickname;
                for (auto it = clients_manager.getClientsBegin(); it != clients_manager.getClientsEnd(); ++it)
                {
                    it->second->send(new_player_packet);
                }

                clients_manager.addClient(rq->iandp, rq->nickname);
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

            if (world->isChunkLoaded(rq->pos))
            {
                ECCPacket p = world->getChunk(rq->pos).getPacket();
                server_socket.send(p, rq->iandp.address, rq->iandp.port);
                world->getEntityManager().sendAddEntityFromAllEntitiesInChunk(rq->pos, clients_manager.getClient(rq->iandp));
            }
            else
                world->requestChunk(rq->pos);
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
        else if (auto rq = request_queue.tryPop<SendMessageRequest>())
        {
            clients_manager.resetClientTimer(rq->iandp);

            if (!clients_manager.isConnected(rq->iandp))
                continue;

            if (rq->message.getSize() && rq->message.toAnsiString()[0] == '/')
            {
                std::string ansi_str = rq->message.toAnsiString();
                // handle server command
                std::string cmd;
                unsigned i = 1;

                while (i < ansi_str.size() && !isspace(ansi_str[i]))
                {
                    cmd += ansi_str[i];
                    ++i;
                }

                std::string args = ansi_str.substr(i);

                handleCommand(clients_manager.getClient(rq->iandp), cmd, args);
            }
            else
            {
                std::string nickname = clients_manager.getClient(rq->iandp).getNickname();
                std::string std_msg;
                for (auto c : rq->message.toUtf8())
                    std_msg += c;
                log(INFO, "<{}>: {}\n", nickname, std_msg);

                ECCPacket msg_packet(Networking::StoC::ReceivedMessage);
                msg_packet << nickname;
                msg_packet << rq->message;

                for (auto it = clients_manager.getClientsBegin(); it != clients_manager.getClientsEnd(); ++it)
                {
                    // don't send it back to the sender!
                    if (it->first != rq->iandp)
                    {
                        it->second->send(msg_packet);
                    }
                }
            }
        }
        else
        {
            log(ERROR, "Unknown CtoS packet request type; ignoring\n");
            request_queue.skip();
        }
    }

    loop:
    ;
}

void Server::passReceiveOnce()
{
    //When we stop the server from outside the receiver, the receive function is blocking so it will wait for something to happen and block
    //To avoid that we send a packet to ourselves to pass the receive once, enough for the while loop of the thread to stop
    ECCPacket p(Networking::CtoS::KeepAlive);

    server_socket.send(p, sf::IpAddress::LocalHost, server_socket.getLocalPort());
}
