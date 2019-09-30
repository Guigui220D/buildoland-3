#pragma once

#include <SFML/Network.hpp>
#include <queue>

#include "../../common-source/Entities/GameEntities/Player.h"

class Server;

struct IpAndPort
{
    inline IpAndPort(sf::IpAddress ip, uint16_t port) :
        address(ip),
        port(port)
    {}

    inline IpAndPort(const IpAndPort& other) :
        address(other.address),
        port(other.port)
    {}

    const sf::IpAddress address;
    const uint16_t port;

    inline bool operator==(const IpAndPort& other) const
    {
        return ((port == other.port) && (address == other.address));
    }

    inline bool operator<(const IpAndPort& other) const
    {
        if (port < other.port)
        {
            return true;
        }
        else
            return address.toInteger() < other.address.toInteger();
    }
};

class Client
{
    public:
        Client(Server* server, IpAndPort ip_and_port, Player* player);
        virtual ~Client();

        inline const IpAndPort& getIpAndPort() const { return ip_and_port; }

        inline bool hasRequestedChunks() const
        {
            sf::Lock lock(chunk_requests_mutex);
            return !chunk_requests.empty();
        }
        sf::Vector2i getNextRequestedChunk();
        void addRequestedChunk(sf::Vector2i chunk);

        inline bool hasPlayer() const { return player; }
        inline Player* getPlayer() const { return player; }

        void send(sf::Packet& packet) const;

    protected:
        IpAndPort ip_and_port;

        std::queue<sf::Vector2i> chunk_requests;
        mutable sf::Mutex chunk_requests_mutex;

        Player* const player;

    private:
        Server* const server;
};
