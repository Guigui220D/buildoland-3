#pragma once

#include "State.h"

#include <memory>

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Thread.hpp>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../common-source/Networking/NetworkRequestQueue.h"
#include "../../common-source/Networking/ServerToClientRequests.h"

namespace TinyProcessLib
{
class Process;
}

namespace sf
{
class Texture;
}

class World;
class EntitiesManager;

class GameState : public State
{
    friend class World;

    public:
        GameState(Game& game, unsigned int id);
        GameState(Game& game, unsigned int id, const std::string& in_nickname, sf::IpAddress server_address, uint16_t server_port);
        ~GameState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

        inline void sendToServer(ECCPacket& packet) { client_socket.send(packet, remote_ip, remote_port); }

    private:
        //Networking stuff
        const bool solo_mode;
        bool connected;
        bool startAndConnectLocalServer();  //For solo mode
        bool handshakeRemoteServer();   //For multiplayer mode
        bool receiveServerHandshake(bool known_port);

        sf::UdpSocket client_socket;
        sf::IpAddress remote_ip;
        uint16_t remote_port;
        sf::Clock heartbeat_clock;

        std::string nickname;

        std::unique_ptr<TinyProcessLib::Process> solo_server_process;

        Networking::StoC::StoCRequestQueue request_queue;
        sf::Thread receiver_thread;
        void receiverLoop();
        void processPacketQueue();

        //World
        std::unique_ptr<World> test_world;
        EntitiesManager& entities;

        //Rendering stuff
        mutable int init_frames_to_skip;
        sf::View base_view;
        bool spyglass_mode = false;
        float zoom = 10.f;
        sf::Thread chunk_vertices_thread;
        mutable sf::Mutex vertex_array_swap_mutex;
        void chunkVerticesGenerationLoop();
        sf::View currentView() const;

        sf::Clock anim_clock;
        int anim_frame = 0;

        sf::Texture const * block_textures;
        sf::Texture const * ground_textures;
        sf::Texture const * item_textures;
        sf::Texture const * ground_details_textures;

        sf::RectangleShape block_pointer;
        sf::RectangleShape block_pointer_side;
        sf::RectangleShape block_pointer_icon;
        sf::RectangleShape hand_item_border;
        sf::Sprite         hand_item_sprite;
        bool bp_volume;
};
