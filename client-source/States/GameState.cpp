#include "GameState.h"

#include "../Game.h"

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>

#include <thread>
#include <chrono>
#include <cmath>

#include "../../common-source/Entities/GameEntities/Player.h"

#include "../Version.h"

#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/CtoS_PlayerActionCodes.h"

#include "../../external/tiny-process-library/process.hpp"

#include "ErrorScreen.h"
#include "InventoryMenuState.h"

#include "../Packets/BreakBlockPacket.h"

#include "../../common-source/Utils/Log.h"

#define YEET break;

GameState::GameState(Game& game, unsigned int id) :
      State(game, id),
      solo_mode(true),
      connected(false),
      remote_ip(sf::IpAddress::LocalHost),
      remote_port(0),
      receiver_thread(&GameState::receiverLoop, this),
      test_world(*this),
      entities(test_world.getEntityManager())
{
    update_transparent = false;
    draw_transparent = false;
}

GameState::GameState(Game& game, unsigned int id, sf::IpAddress server_address, uint16_t server_port) :
      State(game, id),
      solo_mode(false),
      connected(false),
      remote_ip(server_address),
      remote_port(server_port),
      receiver_thread(&GameState::receiverLoop, this),
      test_world(*this),
      entities(test_world.getEntityManager())
{
    update_transparent = false;
    draw_transparent = false;
}

GameState::~GameState()
{
    receiver_thread.terminate();
    if (connected)
    {
        //TEMP
        ECCPacket quit(Networking::CtoS::Disconnect);
        sendToServer(quit);
    }
    if (solo_server_process)
    {
        solo_server_process->kill(true);
    }
}

void GameState::init()
{
    block_textures = &getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    ground_details_textures = &getGame().getResourceManager().getTexture("GROUND_DETAILS");

    block_pointer.setSize(sf::Vector2f(1.f, 1.f));
    block_pointer.setOrigin(sf::Vector2f(.5f, .5f));
    block_pointer.setTexture(&getGame().getResourceManager().getTexture("BLOCK_POINTER"));

    block_pointer_icon.setSize(sf::Vector2f(1.f, 1.f));
    block_pointer_icon.setOrigin(sf::Vector2f(.5f, .5f));
    block_pointer_icon.setTexture(&getGame().getResourceManager().getTexture("BLOCK_POINTER_B"));

    block_pointer_side.setSize(sf::Vector2f(1.f, .5f));
    block_pointer_side.setOrigin(sf::Vector2f(.5f, 0));
    block_pointer_side.setTexture(&getGame().getResourceManager().getTexture("BLOCK_POINTER"));

    my_view = sf::View(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f));

    //Bind to any port
    if (client_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        log(ERROR, "Could not bind client to any port!\n");
        must_be_destroyed = true;
        return;
    }
    log(INFO, "\nBound client to {}\n", client_socket.getLocalPort());

    if (solo_mode)
    {
        if (!startAndConnectLocalServer())
            return;
    }
    else
        if (!handshakeRemoteServer())
        return;

    connected = true;
    client_socket.setBlocking(true);
    receiver_thread.launch();

    test_world.updateChunks(sf::Vector2i());
}

bool GameState::handleEvent(sf::Event& event)
{
    if (getGame().getBindingsManager().pressed("inventory"))
        if (Player::this_player)
            getGame().addStateOnTop(new InventoryMenuState(getGame(), Player::this_player->getInventory(), 0));

    switch (event.type)
    {
        case sf::Event::Resized:
        {
            sf::RenderWindow& window = getGame().getWindow();
            if (window.getSize().x < 200)
                window.setSize(sf::Vector2u(200, window.getSize().y));
            if (window.getSize().y < 200)
                window.setSize(sf::Vector2u(window.getSize().x, 200));
        }
            updateView();
            return false;

        case sf::Event::MouseWheelScrolled:
            break;

        case sf::Event::MouseButtonReleased:
            if (!connected)
                break;
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), my_view);
                world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

                sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

                BreakBlockPacket packet(world_pos_i);
                sendToServer(packet);
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), my_view);
                world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

                sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

                if(Player::this_player)
                    Player::this_player->useHand(world_pos_i);

            }
            break;

        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::A)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    getGame().addStateOnTop(new ErrorState(getGame(), 0, "STATE_INTERRUPTED"));
                    must_be_destroyed = true;
                }
            break;

        default:
            break;
    }
    return true;
}

void GameState::update(float delta_time)
{
    if (Player::this_player)
        my_view.setCenter(Player::this_player->getPosition());

    if (anim_clock.getElapsedTime().asSeconds() >= .5f)
    {
        anim_clock.restart();
        anim_frame = (anim_frame + 1) % 4;
    }

    processPacketQueue();

    test_world.updateLoadedChunk(delta_time);

    entities.updateAll(delta_time);

    {
        sf::Window& window = getGame().getWindow();

        sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(window), my_view);
        world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

        sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

        bp_volume = test_world.getBlockPtr(world_pos_i)->hasVolume(BlockInfo(test_world, world_pos_i));

        block_pointer.setPosition(world_pos);
        block_pointer_icon.setPosition(world_pos);

        if (bp_volume)
        {
            block_pointer.move(sf::Vector2f(0, -.5f));
            block_pointer_icon.move(sf::Vector2f(0, -.5f));
            block_pointer_side.setPosition(world_pos);
        }
    }

    block_pointer_icon.rotate(delta_time * 100.f);

    if (connected && heartbeat_clock.getElapsedTime().asSeconds() >= 5.f)
    {
        heartbeat_clock.restart();
        ECCPacket heartbeat(Networking::CtoS::KeepAlive);
        sendToServer(heartbeat);
    }
}

void GameState::draw(sf::RenderTarget& target) const
{
    target.setView(my_view);

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
    {
        target.draw(i->second->getGroundVertexArray(), ground_textures);
        target.draw(i->second->getGroundDetailsVertexArray(anim_frame), ground_details_textures);
    }

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
        target.draw(i->second->getBlockSidesVertexArray(), block_textures);

    entities.drawAll(target);

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
        target.draw(i->second->getBlockTopsVertexArray(), block_textures);

    entities.drawAllAbove(target);

    if (isTopState())
    {
        target.draw(block_pointer);
        target.draw(block_pointer_icon);

        if (bp_volume)
            target.draw(block_pointer_side);
    }
}

void GameState::updateView()
{
    sf::RenderWindow& window = getGame().getWindow();
    //Resize without deformation
    if (window.getSize().y > window.getSize().x)
    {
        float ratio = (float)window.getSize().y / window.getSize().x;
        float y_size = ratio * zoom;
        my_view.setSize(sf::Vector2f(zoom, y_size));
    }
    else
    {
        float ratio = (float)window.getSize().x / window.getSize().y;
        float x_size = ratio * zoom;
        my_view.setSize(sf::Vector2f(x_size, zoom));
    }
}

void executeProgram(std::string programName) {
    system(programName.c_str());
}

void execute(const std::string& command_line) {
    std::thread worker (executeProgram, command_line);
    worker.detach(); //wait for the worker to complete
}

bool GameState::startAndConnectLocalServer()
{
    assert(solo_mode);

    //Start the server
    {
        std::stringstream strs;
        strs << "bdl-server.exe " << client_socket.getLocalPort();
        strs.flush();
        log(INFO, "Starting server with command {}\n", strs.str());

        solo_server_process = std::make_unique<TinyProcessLib::Process>(strs.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // wait a little to make sure the server is correctly started
        int code;
        if (solo_server_process->try_get_exit_status(code))
        {
            log(ERROR, "Could not start server! Code {}\n", code);
            getGame().addStateOnTop(new ErrorState(getGame(), 0, "SERVER_DIDNT_START"));
            must_be_destroyed = true;
            return false;
        }
    }

    log_prefix_format = "[Client] " + log_prefix_format; // now do this, to help differenciate between the server's and the client's output

    bool handshake = receiveServerHandshake(false);

    return handshake;
}

bool GameState::handshakeRemoteServer()
{
    assert(!solo_mode);

    //TEMP
    //At the moment we send RequestConnection here but this will be done in the connecting to server state
    ECCPacket request(Networking::CtoS::RequestConnection);
    sendToServer(request);

    bool handshake = receiveServerHandshake(true);

    return handshake;
}

bool GameState::receiveServerHandshake(bool known_port)
{
    log(INFO, "Waiting for handshake from server...\n");

    client_socket.setBlocking(false);

    ECCPacket packet; sf::IpAddress address; uint16_t port;

    sf::Clock timeout_clock;

    //Misc handshake info
    char vers[6];
    int seed;   //Maybe this shouldn't be here
    unsigned int player_id;

    while (1)
    {
        sf::Socket::Status status;
        while ((status = client_socket.receive(packet, address, port)) != sf::Socket::Done)
        {
            //Just taking our time
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            if (status == sf::Socket::Disconnected)
            {
                log(ERROR, "Server unreachable\n");
                getGame().addStateOnTop(new ErrorState(getGame(), 0, "SOCKET_DISCONNECTED"));
                must_be_destroyed = true;
                return false;
            } //Server unreachable

            if (timeout_clock.getElapsedTime().asSeconds() >= 5.f)
            {
                log(ERROR, "Time out while waiting for server handshake\n");
                getGame().addStateOnTop(new ErrorState(getGame(), 0, "TIMEOUT_HANDSHAKE", timeout_clock.getElapsedTime().asSeconds()));
                must_be_destroyed = true;
                return false;
            } //Timeout during handshake
        }

        if (address != remote_ip || (known_port && port != remote_port))
        {
            log(ERROR, "Received packet from wrong address.\n");
        } //Wrong address
        else
        {
            if (!known_port)
                remote_port = port;

            int code = 0; packet >> code;

            if (packet && code == Networking::StoC::FinalHandshake)
            {
                packet >> vers;
                vers[5] = 0;

                packet >> seed;
                packet >> player_id;

                if (!packet)
                {
                    log(ERROR, "Malformed handshake packet.\n");
                    getGame().addStateOnTop(new ErrorState(getGame(), 0, "MALFORMED_HANDSHAKE"));
                    must_be_destroyed = true;
                    return false;
                }   //Weird packet
                else
                    break; //Exit the waiting for handshake loop, we got a handshake!
            }
            else
                log(ERROR, "Received wrong packet! Expected handshake code {}.\n", Networking::StoC::FinalHandshake);
            //Not a handshake
        }
    }


    if (std::strcmp(Version::VERSION_SHORT, vers) != 0)
    {
        log(ERROR, "Local server has wrong version! Expected {} but got {}.\n", Version::VERSION_SHORT, vers);
        getGame().addStateOnTop(new ErrorState(getGame(), 0, "WRONG_VERSION"));
        must_be_destroyed = true;
        return false;
    }   //Wrong version

    test_world.setSeed(seed);

    Player::this_player_id = player_id;
    log(INFO, "Received handshake from local server! Its version is {}. Our player has id {}.\n", vers, player_id);

    return true; //Yay
}

void GameState::receiverLoop()
{
    while (true)
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
                log(INFO, "Received a packet from {} : {}, status was DISCONNECTED. Stopping.\n", address.toString(), port); getGame().addStateOnTop(new ErrorState(getGame(), 0, "SOCKET_DISCONNECTED"));
                must_be_destroyed = true;
                break;
            case sf::Socket::Error:
                log(INFO, "Received a packet from {} : {}, status was ERROR.\n", address.toString(), port);
                break;
        }
    }
}

void GameState::processPacketQueue()
{
    using namespace Networking::StoC;

    while (!request_queue.empty())
    {
        if (auto rq = request_queue.tryPop<DisconnectRequest>())
        {
            log(INFO, "Received disconnect code from server.\n");
            getGame().addStateOnTop(new ErrorState(getGame(), 0, "DISCONNECTED_BY_SERVER"));
            must_be_destroyed = true;
        }
        else if (auto rq = request_queue.tryPop<SendChunkRequest>())
        {
            test_world.addChunk(rq->pos, (const char*)rq->chunk_data.data(), rq->chunk_data.size());
        }
        else if (auto rq = request_queue.tryPop<EntityActionRequest>())
        {
            if (!entities.readEntityPacket(rq->data_packet))
                log(ERROR, "Entity packet could not be read.\n");
        }
        else if (auto rq = request_queue.tryPop<BlockUpdateRequest>())
        {
            sf::Vector2i chunk = World::getChunkPosFromBlockPos(rq->pos);
            if (test_world.isChunkLoaded(chunk))
            {
                test_world.getChunk(chunk).setBlock(World::getBlockPosInChunk(rq->pos), getGame().getBlocksManager().getBlockByID(rq->id));
            }
        }
        else if (auto rq = request_queue.tryPop<GroundUpdateRequest>())
        {
            sf::Vector2i chunk = World::getChunkPosFromBlockPos(rq->pos);
            if (test_world.isChunkLoaded(chunk))
            {
                test_world.getChunk(chunk).setGround(World::getBlockPosInChunk(rq->pos), getGame().getGroundsManager().getGroundByID(rq->id));
            }
        }
        else if (auto rq = request_queue.tryPop<PlayerRectificationRequest>())
        {
            if (Player::this_player)
                Player::this_player->setPosition(rq->pos);
        }
        else if (auto rq = request_queue.tryPop<InventoryUpdateRequest>())
        {
            if (Player::this_player)
                Player::this_player->getInventory().handleInventoryUpdateRequest(rq.value());
        }
        else
        {
            log(ERROR, "Uknkown StoC packet request type; ignoring\n");
            request_queue.skip();
        }
    }
}

#undef YEET
