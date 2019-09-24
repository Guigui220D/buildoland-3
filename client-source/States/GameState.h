#pragma once

#include "State.h"

#include <SFML/Network.hpp>

#include "../World/World.h"

class GameState : public State
{
    public:
        GameState(Game* game, unsigned int id = 0);
        GameState(Game* game, unsigned int id, sf::IpAddress server_address, uint16_t server_port);
        ~GameState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

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

        sf::Thread receiver_thread;
        void receiverLoop();

        bool tbd_thread_safe;   //To be destroyed thread safe
        sf::Mutex tbd_mutex;


        //World
        World test_world;

        //Rendering stuff
        sf::View my_view;
        float zoom = 10.f;

        sf::Clock anim_clock;
        int anim_frame = 0;

        sf::Texture const * const block_textures;
        sf::Texture const * const ground_textures;
        sf::Texture const * const ground_details_textures;

        sf::Vector2i test_chunk_pos;
        sf::Vector2i test_next_chunk_pos_turn;
        int test_chunk_next_distance = 1;
        int test_chunk_next_direction = 0;
};
