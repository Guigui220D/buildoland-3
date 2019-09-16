#pragma once

#include "State.h"

#include <SFML/Network.hpp>

#include "../World/World.h"

class GameState : public State
{
    public:
        GameState(Game* game, unsigned int id = 0);
        ~GameState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;

    private:
        //Networking stuff
        const bool solo_mode;
        bool connected;
        bool startAndConnectLocalServer();

        sf::UdpSocket client_socket;
        sf::IpAddress remote_ip;
        uint16_t remote_port;

        sf::Thread receiver_thread;
        void receiverLoop();


        //World
        World test_world;

        //Rendering stuff
        sf::View my_view;
        void updateView();
        float zoom = 10.f;

        sf::Clock anim_clock;
        int anim_frame = 0;

        sf::Texture const * const block_textures;
        sf::Texture const * const ground_textures;
        sf::Texture const * const ground_details_textures;
};
