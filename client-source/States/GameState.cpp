#include "GameState.h"

#include "../Game.h"

#include <cstring>
#include <string>
#include <sstream>

#include <thread>
#include <chrono>
#include <cmath>

#include "../../common-source/Entities/GameEntities/Player.h"

#include "../Version.h"

#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/ServerToClientCodes.h"
#include "../../common-source/Networking/CtoS_PlayerActionCodes.h"

#include "ErrorScreen.h"

#define YEET break;

//TEMPORARY
#include <windows.h>

GameState::GameState(Game* game, unsigned int id, bool show_server_console) :
    State(game, id),
    solo_mode(true),
    connected(false),
    show_console(show_server_console),
    remote_ip(sf::IpAddress::LocalHost),
    remote_port(0),
    receiver_thread(&GameState::receiverLoop, this),
    test_world(this),
    entities(test_world.getEntityManager())
{
    update_transparent = false;
    draw_transparent = false;
}

GameState::GameState(Game* game, unsigned int id, sf::IpAddress server_address, uint16_t server_port) :
    State(game, id),
    solo_mode(false),
    connected(false),
    remote_ip(server_address),
    remote_port(server_port),
    receiver_thread(&GameState::receiverLoop, this),
    test_world(this),
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
        sf::Packet quit;
        quit << (unsigned short)Networking::CtoS::Disconnect;
        sendToServer(quit);
    }
}

void GameState::init()
{
    block_textures = &getGame()->getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame()->getResourceManager().getTexture("GROUND_TEXTURES");
    ground_details_textures = &getGame()->getResourceManager().getTexture("GROUND_DETAILS");

    block_pointer.setSize(sf::Vector2f(1.f, 1.f));
    block_pointer.setOrigin(sf::Vector2f(.5f, .5f));
    block_pointer.setTexture(&getGame()->getResourceManager().getTexture("BLOCK_POINTER"));

    block_pointer_icon.setSize(sf::Vector2f(1.f, 1.f));
    block_pointer_icon.setOrigin(sf::Vector2f(.5f, .5f));
    block_pointer_icon.setTexture(&getGame()->getResourceManager().getTexture("BLOCK_POINTER_B"));

    block_pointer_side.setSize(sf::Vector2f(1.f, .5f));
    block_pointer_side.setOrigin(sf::Vector2f(.5f, 0));
    block_pointer_side.setTexture(&getGame()->getResourceManager().getTexture("BLOCK_POINTER"));

    my_view = sf::View(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f));

    //Bind to any port
    if (client_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cerr << "Could not bind client to any port!" << std::endl;
        must_be_destroyed = true;
        return;
    }
    std::cout << "\nBound client to " << client_socket.getLocalPort() << std::endl;

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
    switch (event.type)
    {
    case sf::Event::Resized:
        {
            sf::RenderWindow& window = getGame()->getWindow();
            if (window.getSize().x < 200)
                window.setSize(sf::Vector2u(200, window.getSize().y));
            if (window.getSize().y < 200)
                window.setSize(sf::Vector2u(window.getSize().x, 200));
        }
        updateView();
        break;

    case sf::Event::MouseWheelScrolled:
        break;

    case sf::Event::MouseButtonReleased:
        if (!connected)
            break;
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f world_pos = getGame()->getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), my_view);
            world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

            sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

            sf::Packet break_packet;
            break_packet << (unsigned short)Networking::CtoS::PlayerAction;
            break_packet << (unsigned short)EntityActions::CtoS::BreakBlock;
            break_packet << world_pos_i.x << world_pos_i.y;

            sendToServer(break_packet);
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
            sf::Vector2f world_pos = getGame()->getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), my_view);
            world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

            sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

            sf::Packet place_packet;
            place_packet << (unsigned short)Networking::CtoS::PlayerAction;
            place_packet << (unsigned short)EntityActions::CtoS::UseItem;
            place_packet << world_pos_i.x << world_pos_i.y;

            sendToServer(place_packet);
        }
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::A)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                getGame()->addStateOnTop(new ErrorState(getGame(), "State interrupted (for testing).", 0));
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

    test_world.updateLoadedChunk();

    entities.updateAll(delta_time);

    {
        sf::Window& window = getGame()->getWindow();

        sf::Vector2f world_pos = getGame()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window), my_view);
        world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));

        sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

        bp_volume = test_world.getBlock(world_pos_i)->hasVolume(BlockInfo(&test_world, world_pos_i));

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
        sf::Packet heartbeat; heartbeat << Networking::CtoS::KeepAlive;
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

    if (!bp_volume)
    {
        target.draw(block_pointer);
        target.draw(block_pointer_icon);
    }

    entities.drawAll(target);

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
        target.draw(i->second->getBlockTopsVertexArray(), block_textures);

    if (bp_volume)
    {
        target.draw(block_pointer);
        target.draw(block_pointer_icon);
        target.draw(block_pointer_side);
    }
}

void GameState::updateView()
{
    sf::RenderWindow& window = getGame()->getWindow();
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

bool GameState::startAndConnectLocalServer()
{
    assert(solo_mode);

    //TODO : Adapt this to other platforms
    //Start the server
    {
        std::stringstream strs;
        strs << "start \"\" \"bdl-server.exe\" " << client_socket.getLocalPort();
        if (!show_console)
            strs << " hide";
        strs.flush();
        std::cout << "Starting server with command " << strs.str() << std::endl;
        int code = system(strs.str().c_str());
        if (code)
        {
            std::cerr << "Could not start server!" << std::endl;
            getGame()->addStateOnTop(new ErrorState(getGame(), "Could not start local server.", 0));
            must_be_destroyed = true;
            return false;
        }
    }

    bool handshake = receiveServerHandshake(false);

    //EWWWWWWWWWWWWWWW
    //Temporary
    SetForegroundWindow(getGame()->getWindow().getSystemHandle());

    return handshake;
}

bool GameState::handshakeRemoteServer()
{
    assert(!solo_mode);

    //TEMP
    //At the moment we send RequestConnection now but this will be done in the connecting to server state
    sf::Packet request;
    request << (unsigned short)Networking::CtoS::RequestConnection;
    sendToServer(request);

    bool handshake = receiveServerHandshake(true);

    return handshake;
}

 bool GameState::receiveServerHandshake(bool known_port)
{
    std::clog << "Waiting for handshake from server..." << std::endl;

    client_socket.setBlocking(false);

    sf::Packet packet; sf::IpAddress address; uint16_t port;
    sf::Clock timeout_clock;

    while (1)
    {
        while (client_socket.receive(packet, address, port) != sf::Socket::Done)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            if (timeout_clock.getElapsedTime().asSeconds() >= 5.f)
            {
                std::cerr << "Time out while waiting for server handshake" << std::endl;
                getGame()->addStateOnTop(new ErrorState(getGame(), "Timeout while waiting for server handshake.", 0));
                must_be_destroyed = true;
                return false;
            }
        }

        if (address != remote_ip || (port != remote_port && known_port))
        {
            std::cerr << "Received packet from wrong address." << std::endl;
        }
        else
        {
            if (!known_port)
                remote_port = port;

            if (packet.getDataSize() == 15)
            {
                unsigned short code = 0; packet >> code;

                if (code != Networking::StoC::FinalHandshake)
                {
                    std::cerr << "Received wrong packet! Expected handshake code " << Networking::StoC::FinalHandshake << " but got " << code << std::endl;
                }
                else
                    break;
            }
            else
                std::cerr << "Received wrong handshake packet! Expected 11 bytes, got " << packet.getDataSize() << '.' << std::endl;
        }
    }

    char vers[6]; packet >> vers;
    vers[5] = 0;

    if (std::strcmp(Version::VERSION_SHORT, vers) != 0)
    {
        std::cerr << "Local server has wrong version! Expected " << Version::VERSION_SHORT << " but got " << vers << '.' << std::endl;
        getGame()->addStateOnTop(new ErrorState(getGame(), "Server has wrong version!", 0));
        must_be_destroyed = true;
        return false;
    }

    unsigned int player_id; packet >> player_id;

    Player::this_player_id = player_id;
    std::clog << "Received handshake from local server! Its version is " << vers << ". Our player has id " << player_id << '.' << std::endl;

    return true;
}

void GameState::receiverLoop()
{
    while (true)
    {
        sf::Packet packet;
        sf::IpAddress address;
        uint16_t port;
        sf::Socket::Status status = client_socket.receive(packet, address, port);

        switch (status)
        {
        case sf::Socket::Done:
            //std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;

            if (address != remote_ip || port != remote_port)
                YEET

            if (packet.getDataSize() >= 2)
            {
                unsigned short code; packet >> code;

                switch (code)
                {
                case Networking::StoC::Disconnect:
                    std::clog << "Received disconnect code from server." << std::endl;
                    getGame()->addStateOnTop(new ErrorState(getGame(), "Disconnected from server.", 0));
                    must_be_destroyed = true;
                    break;

                case Networking::StoC::SendChunk:
                    test_world.addChunk(packet);
                    break;

                case Networking::StoC::EntityAction:
                    if (!entities.readEntityPacket(packet))
                        std::cerr << "Entity packet could not be read." << std::endl;
                    break;

                case Networking::StoC::BlockUpdate:
                    {   //TODO : movee that somewhere else
                        sf::Vector2i pos;
                        uint16_t id;

                        packet >> pos.x >> pos.y;
                        packet >> id;

                        if (!packet)
                        {
                            std::cerr << "Could not read blockUpdate, packet too short" << std::endl;
                            break;
                        }

                        sf::Vector2i chunk = World::getChunkPosFromBlockPos(pos);
                        if (test_world.isChunkLoaded(chunk))
                        {
                            test_world.getChunk(chunk).setBlock(World::getBlockPosInChunk(pos), id);
                        }

                        //std::cout << "Block update at " << pos.y << "; " << pos.y << " | New ID is " << id << std::endl;
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
                            std::cerr << "Could not read groundUpdate, packet too short" << std::endl;
                            break;
                        }

                        sf::Vector2i chunk = World::getChunkPosFromBlockPos(pos);
                        if (test_world.isChunkLoaded(chunk))
                        {
                            test_world.getChunk(chunk).setGround(World::getBlockPosInChunk(pos), id);
                        }
                    }
                    break;

                case Networking::StoC::PlayerRectification:
                    {
                        sf::Vector2f pos;
                        packet >> pos.x >> pos.y;

                        if (!packet)
                            break;

                        if (!Player::this_player)
                            break;

                        Player::this_player->setPosition(pos);
                    }
                    break;

                default:
                    std::cerr << "Unknown packet code" << std::endl;
                    break;
                }
            }
            else

            break;
        case sf::Socket::NotReady:
            //std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was NOT READY." << std::endl;
            break;
        case sf::Socket::Partial:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was PARTIAL." << std::endl;
            break;
        case sf::Socket::Disconnected:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was DISCONNECTED. Stopping." << std::endl;
            getGame()->addStateOnTop(new ErrorState(getGame(), "Server socket unreachable.", 0));
            must_be_destroyed = true;
            break;
        case sf::Socket::Error:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was ERROR." << std::endl;
            break;
        }
    }
}

#undef YEET
