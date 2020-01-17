#pragma once

#include "State.h"

#include <SFML/Network.hpp>

#include "../World/World.h"

class GameState : public State
{
    friend class World;

    public:
        GameState(Game* game, unsigned int id, bool show_server_console);
        GameState(Game* game, unsigned int id, sf::IpAddress server_address, uint16_t server_port);
        ~GameState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

        inline void sendToServer(sf::Packet& packet) { client_socket.send(packet, remote_ip, remote_port); }

    private:
        //Networking stuff
        const bool solo_mode;
        bool connected;
        bool show_console;
        bool startAndConnectLocalServer();  //For solo mode
        bool handshakeRemoteServer();   //For multiplayer mode
        bool receiveServerHandshake(bool known_port);

        sf::UdpSocket client_socket;
        sf::IpAddress remote_ip;
        uint16_t remote_port;
        sf::Clock heartbeat_clock;

        sf::Thread receiver_thread;
        void receiverLoop();

        //World
        World test_world;
        EntitiesManager& entities;

        //Rendering stuff
        sf::View my_view;
        float zoom = 10.f;

        sf::Clock anim_clock;
        int anim_frame = 0;

        sf::Texture const * block_textures;
        sf::Texture const * ground_textures;
        sf::Texture const * ground_details_textures;

        sf::RectangleShape block_pointer;
        sf::RectangleShape block_pointer_side;
        sf::RectangleShape block_pointer_icon;
        bool bp_volume;
};
