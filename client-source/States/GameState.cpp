#include "GameState.h"

#include "../Game.h"

#include <cstring>
#include <string>
#include <sstream>

#include <cmath>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Blocks/Block.h"
#include "../../common-source/Blocks/GameBlocks.h"
#include "../../common-source/Grounds/GameGrounds.h"
#include "../../common-source/Items/Item.h"

#include "../../common-source/TileEntities/TileEntity.h"

#include "../Version.h"

#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Networking/CtoS_PlayerActionCodes.h"
#include "../../common-source/Networking/ServerToClientCodes.h"

#include "../Res/ResourceManager.h"
#include "../Settings/BindingsManager.h"
#include "../../common-source/Items/ItemsRegister.h"

#include "../../external/tiny-process-library/process.hpp"

#include "ErrorScreen.h"
#include "InventoryMenuState.h"
#include "../../common-source/Networking/StoC_InventoryUpdateCodes.h"

#include "../Packets/BreakBlockPacket.h"

#include "../World/World.h"

#include "../../common-source/Utils/Log.h"


#define CHUNK_VERTICES_ASYNC 1

GameState::GameState(Game& game, unsigned int id) :
      State(game, id),
      solo_mode(true),
      connected(false),
      remote_ip(sf::IpAddress::LocalHost),
      remote_port(0),
      nickname("Me"),
      receiver_thread(&GameState::receiverLoop, this),
      test_world(std::make_unique<World>(*this)),
      entities(test_world->getEntityManager()),
      init_frames_to_skip(20),
      chunk_vertices_thread(&GameState::chunkVerticesGenerationLoop, this)
{
    Player::this_player = nullptr;
    Player::this_player_id = 0;

    update_transparent = false;
    draw_transparent = false;
}

GameState::GameState(Game& game, unsigned int id, const std::string& in_nickname, sf::IpAddress server_address, uint16_t server_port) :
      State(game, id),
      solo_mode(false),
      connected(false),
      remote_ip(server_address),
      remote_port(server_port),
      nickname(in_nickname),
      receiver_thread(&GameState::receiverLoop, this),
      test_world(std::make_unique<World>(*this)),
      entities(test_world->getEntityManager()),
      init_frames_to_skip(20),
      chunk_vertices_thread(&GameState::chunkVerticesGenerationLoop, this)
{
    Player::this_player = nullptr;
    Player::this_player_id = 0;

    update_transparent = false;
    draw_transparent = false;
}

GameState::~GameState()
{
#ifdef CHUNK_VERTICES_ASYNC
    chunk_vertices_thread.terminate();
#endif
    receiver_thread.terminate();
    if (connected)
    {
        //TEMP
        ECCPacket quit(Networking::CtoS::Disconnect);
        sendToServer(quit);
    }
    if (solo_server_process)
    {
        solo_server_process->get_exit_status();
    }

    log_prefix_format = "";
}

void GameState::init()
{
    block_textures = &getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    item_textures = &getGame().getResourceManager().getTexture("ITEM_TEXTURES");
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

    base_view = sf::View(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f));

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
#if CHUNK_VERTICES_ASYNC
    chunk_vertices_thread.launch();
#endif

    test_world->updateChunks(sf::Vector2i());
}

bool GameState::handleEvent(sf::Event& event)
{
    if (getGame().getBindingsManager().pressed("inventory"))
    {
        if (Player::this_player)
            getGame().addStateOnTop(new InventoryMenuState(getGame(), Player::this_player->getInventory(), 0));
    }
    else if (getGame().getBindingsManager().pressed("throw_item"))
    {
        Player::this_player->getInventory().dropHand();
    }

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
                sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), currentView());

                sf::Vector2i world_pos_i(std::round(world_pos.x), std::round(world_pos.y));

                int on_entity = false;
                for (auto& entity : entities.entities_map)
                {
                    sf::FloatRect world_hitbox(entity.second->getPosition() - entity.second->getVisualHitbox()/2.f, entity.second->getVisualHitbox());
                    if (Player::this_player && world_hitbox.contains(world_pos))
                    {
                        //log(INFO, "left click on entity {}\n", entity.first);
                        entity.second->onLeftClick(*Player::this_player);
                        //on_entity = true;
                    }
                }

                if (!on_entity)
                {
                    BreakBlockPacket packet(world_pos_i);
                    sendToServer(packet);
                }
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), currentView());

                sf::Vector2i world_pos_i(std::round(world_pos.x), std::round(world_pos.y));

                int on_entity = false;
                for (auto& entity : entities.entities_map)
                {
                    sf::FloatRect world_hitbox(entity.second->getPosition() - entity.second->getVisualHitbox()/2.f, entity.second->getVisualHitbox());
                    if (Player::this_player && world_hitbox.contains(world_pos))
                    {
                        //log(INFO, "right click on entity {}\n", entity.first);
                        entity.second->onRightClick(*Player::this_player);
                        //on_entity = true;
                    }
                }

                if(!on_entity && Player::this_player)
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
    {
        base_view.setCenter(Player::this_player->getPosition());

        if (Player::this_player->getInventory().contents.at(0))
        {
            auto& item = *Player::this_player->getInventory().contents.at(0).getItem();
            switch (item.getTexturesSet())
            {
                default:
                case Item::ItemsTextureSet:
                    hand_item_sprite.setTexture(*item_textures);
                    break;
                case Item::GroundsTextureSet:
                    hand_item_sprite.setTexture(*ground_textures);
                    break;
                case Item::BlocksTextureSet:
                    hand_item_sprite.setTexture(*block_textures);
                    break;
            }
            hand_item_sprite.setTextureRect(item.getTexture(Player::this_player->getInventory().contents.at(0)));
            hand_item_sprite.setScale(2.f, 2.f);
            hand_item_sprite.setOrigin(hand_item_sprite.getLocalBounds().width/2.f, 0.f);
            hand_item_sprite.setPosition(getGame().getWindow().getSize().x/2.f,
                                         getGame().getWindow().getSize().y - hand_item_sprite.getLocalBounds().height*2.f - 10.f);

            hand_item_border.setFillColor(sf::Color(200, 200, 200, 100));
            hand_item_border.setOutlineColor(sf::Color(255, 255, 255, 240));
            hand_item_border.setOutlineThickness(2.f);
            hand_item_border.setSize(sf::Vector2f(18.f*2.f + 0.f, 18.f*2.f + 0.f));
            hand_item_border.setPosition(hand_item_sprite.getPosition() + sf::Vector2f(2.f, -2.f));
            hand_item_border.setOrigin(hand_item_border.getLocalBounds().width/2.f, 0.f);
        }
    }

    if (anim_clock.getElapsedTime().asSeconds() >= .5f)
    {
        anim_clock.restart();
        anim_frame = (anim_frame + 1) % 4;
    }

    processPacketQueue();

    test_world->updateLoadedChunk(delta_time);

    entities.updateAll(delta_time);
    test_world->updateTileEntities(delta_time);

    spyglass_mode = Player::this_player && Player::this_player->getInventory().contents.at(0).getItem() == ItemsRegister::SPYGLASS;

    {
        sf::Window& window = getGame().getWindow();

        sf::Vector2f world_pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(window), currentView());

        for (auto& entity : entities.entities_map)
        {
            sf::FloatRect world_hitbox(entity.second->getPosition() - entity.second->getVisualHitbox()/2.f, entity.second->getVisualHitbox());
            entity.second->highlighted = world_hitbox.contains(world_pos);
        }

        world_pos = sf::Vector2f(std::round(world_pos.x), std::round(world_pos.y));
        sf::Vector2i world_pos_i(world_pos.x, world_pos.y);

        bp_volume = test_world->getBlockPtr(world_pos_i)->hasVolume(TileReference(world_pos_i, *test_world));

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
    target.setView(currentView());

    if (init_frames_to_skip > 0)
    {
        --init_frames_to_skip;
        return;
    }

    {
#if CHUNK_VERTICES_ASYNC
        sf::Lock lock(vertex_array_swap_mutex);
#else
        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
        {
            if (i->second->vertexArraysOutOfDate())
            {
                i->second->generateVertices();
                i->second->swapVertexArrays();
            }
        }
#endif

        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
        {
            target.draw(i->second->getGroundVertexArray(), ground_textures);
            target.draw(i->second->getGroundDetailsVertexArray(anim_frame), ground_details_textures);
        }

        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
            target.draw(i->second->getBlockSidesVertexArray(), block_textures);

        entities.sortDrawOrder();
        entities.drawAllBelow(target);
        entities.drawAll(target);
        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
            for (std::shared_ptr<TileEntity>& te : i->second->actual_tile_entities)
                te->draw(target);

        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
            target.draw(i->second->getBlockTopsVertexArray(), block_textures);

    }

    entities.drawAllAbove(target);
    for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
            for (std::shared_ptr<TileEntity>& te : i->second->actual_tile_entities)
                te->drawAbove(target);

    if (isTopState())
    {
        target.draw(block_pointer);
        target.draw(block_pointer_icon);

        if (bp_volume)
            target.draw(block_pointer_side);
    }

    getGame().useDefaultView();
    target.draw(hand_item_border);
    if (Player::this_player && Player::this_player->getInventory().contents.at(0))
    {
        target.draw(hand_item_sprite);
    }

    // draw entities' bounding boxes

    /*
    for (auto& entity : entities.entities_map)
    {
        sf::RectangleShape shape;
        shape.setSize(entity.second->getVisualHitbox());
        shape.setPosition(entity.second->getPosition() - entity.second->getVisualHitbox()/2.f);
        shape.setOutlineThickness(0.1f);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(sf::Color::Transparent);

    target.draw(shape);
}
*/
}

void GameState::updateView()
{
    sf::RenderWindow& window = getGame().getWindow();
    //Resize without deformation
    if (window.getSize().y > window.getSize().x)
    {
        float ratio = (float)window.getSize().y / window.getSize().x;
        float y_size = ratio * zoom;
        base_view.setSize(sf::Vector2f(zoom, y_size));
    }
    else
    {
        float ratio = (float)window.getSize().x / window.getSize().y;
        float x_size = ratio * zoom;
        base_view.setSize(sf::Vector2f(x_size, zoom));
    }
}

void executeProgram(std::string programName)
{
    system(programName.c_str());
}

void execute(const std::string& command_line)
{
    std::thread worker(executeProgram, command_line);
    worker.detach(); //wait for the worker to complete
}

bool GameState::startAndConnectLocalServer()
{
    assert(solo_mode);

    //Start the server
    {
        std::stringstream strs;
#ifdef _WIN32
        strs << "bdl-server.exe " << client_socket.getLocalPort();
#else
        strs << "./bdl-server " << client_socket.getLocalPort();
#endif
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

    log_prefix_format = "[Client] "; // now do this, to help differenciate between the server's and the client's output

    bool handshake = receiveServerHandshake(false);

    return handshake;
}

bool GameState::handshakeRemoteServer()
{
    assert(!solo_mode);

    //TEMP
    //At the moment we send RequestConnection here but this will be done in the connecting to server state
    ECCPacket request(Networking::CtoS::RequestConnection);
    request << nickname;
    sendToServer(request);

    bool handshake = receiveServerHandshake(true);

    return handshake;
}

bool GameState::receiveServerHandshake(bool known_port)
{
    log(INFO, "Waiting for handshake from server...\n");

    client_socket.setBlocking(false);

    ECCPacket inventory_placeholder;
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

            if (packet)
            {
                if (code == Networking::StoC::ConnectionRefused)
                {
                    uint8_t reason; // Networking::StoC::ConnectionRefusalReason
                    packet >> reason;

                    if (!packet)
                    {
                        log(ERROR, "Received ill-formed packet! Couldn't get data.\n");
                        getGame().addStateOnTop(new ErrorState(getGame(), 0, "MALFORMED_HANDSHAKE"));
                        must_be_destroyed = true;
                        return false;
                    }   //Weird packet
                    else
                    {
                        log(ERROR, "Failure, reason code {}\n", reason);
                        switch (reason)
                        {
                            case Networking::StoC::InvalidNickname:
                                getGame().addStateOnTop(new ErrorState(getGame(), 0, "INVALID_NICKNAME", timeout_clock.getElapsedTime().asSeconds()));
                                break;
                            case Networking::StoC::NicknameAlreadyChosen:
                                getGame().addStateOnTop(new ErrorState(getGame(), 0, "NICKNAME_ALREADY_TAKEN", timeout_clock.getElapsedTime().asSeconds()));
                                break;
                            default:
                                getGame().addStateOnTop(new ErrorState(getGame(), 0, "DISCONNECTED_BY_SERVER", timeout_clock.getElapsedTime().asSeconds()));
                                break;
                        }
                        must_be_destroyed = true;
                        return false;
                    }
                }
                else if (code == Networking::StoC::FinalHandshake)
                {
                    packet >> vers;
                    vers[5] = 0;

                    packet >> seed;
                    packet >> player_id;

                    if (!packet)
                    {
                        log(ERROR, "Received ill-formed packet! Couldn't get data.\n");
                        getGame().addStateOnTop(new ErrorState(getGame(), 0, "MALFORMED_HANDSHAKE"));
                        must_be_destroyed = true;
                        return false;
                    }   //Weird packet
                    else
                        break; //Exit the waiting for handshake loop, we got a handshake!
                }
                else
                {
                    if (code == Networking::StoC::InventoryUpdate)
                    {
                        log(INFO, "TEST : got inventory packet before handshake.\n");
                        inventory_placeholder = packet;
                    }
                    else
                        log(ERROR, "Received wrong packet! Expected handshake code {} (handshake) or {} (inventory initialization) but got {}.\n", Networking::StoC::FinalHandshake, Networking::StoC::InventoryUpdate, code);
                }

            }
            else
                log(ERROR, "Received ill-formed packet! Couldn't get type-code.\n");
        }
    }


    if (std::strcmp(Version::VERSION_SHORT, vers) != 0)
    {
        log(ERROR, "Local server has wrong version! Expected {} but got {}.\n", Version::VERSION_SHORT, vers);
        getGame().addStateOnTop(new ErrorState(getGame(), 0, "WRONG_VERSION"));
        must_be_destroyed = true;
        return false;
    }   //Wrong version


    if (inventory_placeholder && !inventory_placeholder.isCorrupted())
    {
        Networking::StoC::InventoryUpdateRequest rq;
        inventory_placeholder >> rq.type;

        if (rq.type == InventoryUpdates::StoC::SetInventory)
        {
            for (int i = 0; i < 25; i++)
            {
                inventory_placeholder >> rq.stack_list[i];
                if (!packet)
                    break;
            }

            if (inventory_placeholder)
                request_queue.pushRequest(std::move(rq));
        }
    }

    test_world->setSeed(seed);

    Player::this_player_id = player_id;
    log(INFO, "Received handshake from local server! Its version is {}. Our player has id {}.\n", vers, player_id);

    return true; //Yay
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
            test_world->addChunk(rq->pos, (const char*)rq->chunk_data.data(), rq->chunk_data.size());
        }
        else if (auto rq = request_queue.tryPop<EntityActionRequest>())
        {
            if (!entities.readEntityPacket(rq->data_packet))
                log(ERROR, "Entity packet could not be read.\n");
        }
        else if (auto rq = request_queue.tryPop<TileEntityUpdateRequest>())
        {
            sf::Vector2i pos;

            rq->data_packet >> pos.x;
            rq->data_packet >> pos.y;

            if (!test_world->findTEandGivePacket(pos, rq->data_packet))
                log(ERROR, "Tile entity packet couldn't be passed to TE.\n");
        }
        else if (auto rq = request_queue.tryPop<BlockUpdateRequest>())
        {
            sf::Vector2i chunk = World::getChunkPosFromBlockPos(rq->pos);
            if (test_world->isChunkLoaded(chunk))
            {
                test_world->getChunk(chunk).setBlock(World::getBlockPosInChunk(rq->pos), getGame().getBlocksManager().getBlockByID(rq->id));
            }
        }
        else if (auto rq = request_queue.tryPop<GroundUpdateRequest>())
        {
            sf::Vector2i chunk = World::getChunkPosFromBlockPos(rq->pos);
            if (test_world->isChunkLoaded(chunk))
            {
                test_world->getChunk(chunk).setGround(World::getBlockPosInChunk(rq->pos), getGame().getGroundsManager().getGroundByID(rq->id));
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
            else
                request_queue.pushRequest(std::move(rq.value()));
        }
        else
        {
            log(ERROR, "Uknkown StoC packet request type; ignoring\n");
            request_queue.skip();
        }
    }
}

void GameState::chunkVerticesGenerationLoop()
{
    // the shared_ptr is used so that if World removes a chunk, we can still access it until we clear the rendered_chunk_list
    // shared_ptr reference counting is atomic, no need for locks around shared_ptr
    std::vector<std::shared_ptr<Chunk>> rendered_chunk_list; // used to prevent race conditions if chunks are added during the async rendering loop
    while (true)
    {
        rendered_chunk_list.resize(0); // don't deallocate

        test_world->chunkDeletionLock();
        for (auto i = test_world->getChunksBegin(); i != test_world->getChunksEnd(); i++)
        {
            rendered_chunk_list.push_back(i->second);
        }
        test_world->chunkDeletionUnlock();

        for (const auto& chunk : rendered_chunk_list)
        {
            if (chunk->vertexArraysOutOfDate())
            {
                chunk->generateVertices();
                sf::Lock lock_2(vertex_array_swap_mutex);
                chunk->swapVertexArrays();
            }
        }
    }
}

sf::View GameState::currentView() const
{
    if (spyglass_mode)
    {
        sf::View spyglass_view = base_view;
        spyglass_view.zoom(1.5f);
        return spyglass_view;
    }
    else
    {
        return base_view;
    }
}


