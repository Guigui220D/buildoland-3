#pragma once

#include "State.h"

#include <SFML/Network.hpp>

#include "../World/World.h"
#include "../../common-source/Networking/NetworkRequestQueue.h"
#include "../../common-source/Networking/ServerToClientRequests.h"

#include <mutex>
#include <condition_variable>

class semaphore
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.

public:
    void notify() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while(!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }

    bool try_wait() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }
};

namespace TinyProcessLib
{
class Process;
}

class GameState : public State
{
    friend class World;

    public:
        GameState(Game& game, unsigned int id);
        GameState(Game& game, unsigned int id, sf::IpAddress server_address, uint16_t server_port);
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

        std::unique_ptr<TinyProcessLib::Process> solo_server_process;

        Networking::StoC::StoCRequestQueue request_queue;
        sf::Thread receiver_thread;
        void receiverLoop();
        void processPacketQueue();

        //World
        World test_world;
        EntitiesManager& entities;

        //Rendering stuff
        mutable int init_frames_to_skip;
        sf::View my_view;
        float zoom = 10.f;
        sf::Thread chunk_vertices_thread;
        mutable sf::Mutex vertex_array_swap_mutex;
        void chunkVerticesGenerationLoop();

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
