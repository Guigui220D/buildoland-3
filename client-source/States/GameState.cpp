#include "GameState.h"

#include "../Game.h"

#include <cstring>
#include <string>
#include <sstream>

#include "../Version.h"

#include "../../common/Networking/ClientToServerCodes.h"
#include "../../common/Networking/ServerToClientCodes.h"

//TEMPORARY
#include <windows.h>

GameState::GameState(Game* game, unsigned int id) :
    State(game, id),
    solo_mode(true),
    connected(false),
    receiver_thread(&GameState::receiverLoop, this),
    tbd_thread_safe(false),
    test_world(game),
    my_view(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f)),
    block_textures(&getGame()->getResourceManager().getTexture("BLOCK_TEXTURES")),
    ground_textures(&getGame()->getResourceManager().getTexture("GROUND_TEXTURES")),
    ground_details_textures(&getGame()->getResourceManager().getTexture("GROUND_DETAILS"))
{
    update_transparent = false;
    draw_transparent = false;
    updateView();
}

GameState::~GameState()
{
    receiver_thread.terminate();
    if (connected)
    {
        //TEMP
        sf::Packet quit;
        quit << (unsigned short)Networking::CtoS::Disconnect;
        client_socket.send(quit, remote_ip, remote_port);
    }
}

void GameState::init()
{
    test_next_chunk_pos_turn = sf::Vector2i(0, -1);
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
    {

    }

    connected = true;
    client_socket.setBlocking(true);
    receiver_thread.launch();
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


    case sf::Event::KeyPressed:
        //TEST
        if (event.key.code == sf::Keyboard::A)
        {
            switch (test_chunk_next_direction)
            {
            case 0:
                test_chunk_pos += sf::Vector2i(0, -1);
                if (test_chunk_pos == test_next_chunk_pos_turn)
                {
                    test_next_chunk_pos_turn = test_chunk_pos + sf::Vector2i(test_chunk_next_distance, 0);
                    test_chunk_next_direction++;
                    test_chunk_next_direction %= 4;
                }
                break;
            case 1:
                test_chunk_pos += sf::Vector2i(1, 0);
                if (test_chunk_pos == test_next_chunk_pos_turn)
                {
                    test_chunk_next_distance++;
                    test_next_chunk_pos_turn = test_chunk_pos + sf::Vector2i(0, test_chunk_next_distance);
                    test_chunk_next_direction++;
                    test_chunk_next_direction %= 4;
                }
                break;
            case 2:
                test_chunk_pos += sf::Vector2i(0, 1);
                if (test_chunk_pos == test_next_chunk_pos_turn)
                {
                    test_next_chunk_pos_turn = test_chunk_pos + sf::Vector2i(-test_chunk_next_distance, 0);
                    test_chunk_next_direction++;
                    test_chunk_next_direction %= 4;
                }
                break;
            case 3:
                test_chunk_pos += sf::Vector2i(-1, 0);
                if (test_chunk_pos == test_next_chunk_pos_turn)
                {
                    test_chunk_next_distance++;
                    test_next_chunk_pos_turn = test_chunk_pos + sf::Vector2i(0, -test_chunk_next_distance);
                    test_chunk_next_direction++;
                    test_chunk_next_direction %= 4;
                }
                break;
            }



            std::cout << "Requesting chunk " << test_chunk_pos.x << ", " << test_chunk_pos.y << std::endl;

            sf::Packet request;
            request << (unsigned short)Networking::CtoS::RequestChunk;
            request << test_chunk_pos.x << test_chunk_pos.y;
            client_socket.send(request, remote_ip, remote_port);
        }
        break;

    case sf::Event::MouseWheelScrolled:
        zoom += event.mouseWheelScroll.delta;
        updateView();
        break;

    default:
        break;
    }
    return true;
}

void GameState::update(float delta_time)
{
    tbd_mutex.lock();
    if (tbd_thread_safe)
        must_be_destroyed = true;
    tbd_mutex.unlock();

    //TEMPORARY
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(-5.f * delta_time, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(5.f * delta_time, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(0, 5.f * delta_time));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(0, -5.f * delta_time));

    if (anim_clock.getElapsedTime().asSeconds() >= .5f)
    {
        anim_clock.restart();
        anim_frame = (anim_frame + 1) % 4;
    }

    test_world.updateLoadedChunk(my_view.getCenter());
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

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
        target.draw(i->second->getBlockTopsVertexArray(), block_textures);
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
        strs << "start \"\" \"bdl-server.exe\" " << client_socket.getLocalPort(); strs.flush();
        std::cout << "Starting server with command " << strs.str() << std::endl;
        int code = system(strs.str().c_str());
        if (code)
        {
            std::cerr << "Could not start server!" << std::endl;
            must_be_destroyed = true;
            return false;
        }
    }

    std::cout << "Server started : waiting for handshake..." << std::endl;

    //Handshake with server
    client_socket.setBlocking(false);

    sf::Packet packet; sf::IpAddress address; uint16_t port;
    sf::Clock timeout_clock;

    while (client_socket.receive(packet, address, port) != sf::Socket::Done)
    {
        if (timeout_clock.getElapsedTime().asSeconds() >= 5.f)
        {
            std::cerr << "Time out while waiting for server handshake" << std::endl;
            must_be_destroyed = true;
            return false;
        }
    }

    if (address != sf::IpAddress::LocalHost)
    {
        std::cerr << "Received packet from wrong address!" << std::endl;
        must_be_destroyed = true;
        return false;
    }
    if (packet.getDataSize() != 11)
    {
        std::cerr << "Received wrong packet! Expected 13 bytes, got " << packet.getDataSize() << '.' << std::endl;
        must_be_destroyed = true;
        return false;
    }

    unsigned short code = 0; packet >> code;

    if (code != Networking::StoC::SoloHandshake)
    {
        std::cerr << "Received wrong packet! Expected handshake code " << Networking::StoC::SoloHandshake << " but got " << code << std::endl;
        must_be_destroyed = true;
        return false;
    }

    char vers[6];
    packet >> vers;
    vers[5] = 0;

    if (std::strcmp(Version::VERSION_SHORT, vers) != 0)
    {
        std::cerr << "Local server has wrong version! Expected " << Version::VERSION_SHORT << " but got " << vers << '.' << std::endl;
        must_be_destroyed = true;
        return false;
    }

    std::cout << "Received handshake from local server! Its version is " << vers << std::endl;
    std::cout << std::endl;

    remote_ip = address;
    remote_port = port;

    //EWWWWWWWWWWWWWWW
    //Temporary
    SetForegroundWindow(getGame()->getWindow().getSystemHandle());

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
            std::clog << "Received a " << packet.getDataSize() << " bytes packet from " << address.toString() << ':' << port << std::endl;

            if (packet.getDataSize() >= 2)
            {
                unsigned short code; packet >> code;

                switch (code)
                {
                case Networking::StoC::Disconnect:
                    std::cout << "Received disconnect code from server." << std::endl;
                    tbd_mutex.lock();
                    tbd_thread_safe = true;
                    tbd_mutex.unlock();
                    break;
                case Networking::StoC::SendChunk:
                    test_world.addChunk(packet);
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
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was DISCONNECTED." << std::endl;
            break;
        case sf::Socket::Error:
            std::clog << "Received a packet from " << address.toString() << ':' << port << ", status was ERROR." << std::endl;
            break;
        }
    }
}
