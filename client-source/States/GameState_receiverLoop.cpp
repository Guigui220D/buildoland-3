#include "GameState.h"

#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/StoC_InventoryUpdateCodes.h"

#include "../../common-source/Utils/Log.h"

#include "../Game.h"
#include "../World/Chunk.h"
#include "ErrorScreen.h"

#define YEET break; //YEET sounds more violent than break

void GameState::receiverLoop()
{
    while (!stop_receiver_thread)
    {
        ECCPacket packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = client_socket.receive(packet, address, port);

        switch (status)
        {
            case sf::Socket::Done:
                //std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;

                if (address != remote_ip || port != remote_port)
                    YEET

                if (packet.isCorrupted())
                    {
                        log(ERROR, "Received a corrupted packet from the server, ignoring it.\n");
                        YEET
                    }

                if (packet.getDataSize() >= 2)
                {
                    int code; packet >> code;

                    switch (code)
                    {
                        case Networking::StoC::Disconnect:
                            request_queue.pushRequest(Networking::StoC::DisconnectRequest{});
                            break;

                        case Networking::StoC::SendChunk:
                        {
                            //We expect the packet to be of that size
                            //4 bytes per tile (2 for block and 2 for ground)
                            //8 bytes for the position
                            //2 bytes for the packet header
                            size_t expected_packet_size = Chunk::getChunkDataSize();
                            expected_packet_size += sizeof(int) * 2; //Position
                            expected_packet_size += 2;

                            if (packet.getDataSize() < expected_packet_size)
                            {
                                log(ERROR, "Chunk packet is too small! Expected {} bytes, got {} bytes.\n", expected_packet_size, packet.getDataSize());
                                break;
                            }

                            //Get chunk position
                            sf::Vector2i pos;
                            packet >> pos.x;
                            packet >> pos.y;

                            Networking::StoC::SendChunkRequest rq;
                            rq.pos = pos;
                            rq.chunk_data.resize(packet.getDataSize());
                            std::memcpy(rq.chunk_data.data(), packet.getData(), rq.chunk_data.size());

                            request_queue.pushRequest(std::move(rq));
                            break;
                        }

                        case Networking::StoC::EntityAction:
                            request_queue.pushRequest(Networking::StoC::EntityActionRequest{packet});
                            break;

                        case Networking::StoC::TileEntityUpdate:
                            request_queue.pushRequest(Networking::StoC::TileEntityUpdateRequest{packet});
                            break;

                        case Networking::StoC::BlockUpdate:
                        {   //TODO : movee that somewhere else
                            sf::Vector2i pos;
                            uint16_t id;

                            packet >> pos.x >> pos.y;
                            packet >> id;

                            if (!packet)
                            {
                                log(ERROR, "Could not read blockUpdate, packet too short\n");
                                break;
                            }

                            request_queue.pushRequest(Networking::StoC::BlockUpdateRequest{pos, id});
                        }
                        break;

                        case Networking::StoC::GroundUpdate:
                        {
                            sf::Vector2i pos;
                            uint16_t id;

                            packet >> pos.x >> pos.y;
                            packet >> id;

                            if (!packet)
                            {
                                log(ERROR, "Could not read groundUpdate, packet too short\n");
                                break;
                            }

                            request_queue.pushRequest(Networking::StoC::GroundUpdateRequest{pos, id});
                        }
                        break;

                        case Networking::StoC::PlayerRectification:
                        {
                            sf::Vector2f pos;
                            packet >> pos.x >> pos.y;

                            if (!packet)
                                break;

                            request_queue.pushRequest(Networking::StoC::PlayerRectificationRequest{pos});
                        }
                        break;

                        case Networking::StoC::InventoryUpdate:
                        {
                            Networking::StoC::InventoryUpdateRequest rq;
                            packet >> rq.type;

                            if (!packet)
                            {
                                log(ERROR, "Could not read inventory update, packet too short to get type.\n");
                                break;
                            }

                            switch (rq.type)
                            {
                                case InventoryUpdates::StoC::AddStack:
                                    packet >> rq.stack_add;
                                    break;
                                case InventoryUpdates::StoC::SetStack:
                                    packet >> rq.pos;
                                    if (!packet)
                                        break;
                                    packet >> rq.stack_set;
                                    break;
                                case InventoryUpdates::StoC::SetInventory:
                                    for (int i = 0; i < 25; i++)
                                    {
                                        packet >> rq.stack_list[i];
                                        if (!packet)
                                            break;
                                    }
                                    break;
                            }

                            request_queue.pushRequest(std::move(rq));
                        }
                        break;

                        default:
                            log(ERROR, "Unknown packet code\n");
                            break;
                    }
                }
                else
                    ;
                break;
            case sf::Socket::NotReady:
                //log(INFO, "Received a packet from {} : {}, status was NOT READY.\n", address.toString(), port);
                break;
            case sf::Socket::Partial:
                log(INFO, "Received a packet from {} : {}, status was PARTIAL.\n", address.toString(), port);
                break;
            case sf::Socket::Disconnected:
                log(INFO, "Received a packet from {} : {}, status was DISCONNECTED. Stopping.\n", address.toString(), port);
                if (!must_be_destroyed)
                    getGame().addStateOnTop(new ErrorState(getGame(), 0, "SOCKET_DISCONNECTED"));
                must_be_destroyed = true;
                break;
            case sf::Socket::Error:
                log(INFO, "Received a packet from {} : {}, status was ERROR.\n", address.toString(), port);
                break;
        }
    }
}

#undef YEET
