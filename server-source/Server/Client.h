#pragma once

#include <SFML/System/Clock.hpp>

#include <vector>

#include "../../common-source/Networking/ECCPacket.h"
#include "../../common-source/Networking/IpAndPort.h"

class Server;
class Player;

class Client
{
    public:
        Client(Server& server, IpAndPort ip_and_port, Player* player);
        virtual ~Client();

        inline const IpAndPort& getIpAndPort() const { return ip_and_port; }

        inline bool hasPlayer() const { return player; }
        inline Player* getPlayer() const { return player; }
        inline void setPlayer(Player* new_player) { player =  new_player; }

        void send(ECCPacket& packet) const;

        inline bool operator==(const Client& other) const
        {
            return ip_and_port == other.ip_and_port;
        }

        sf::Clock last_packet_received;

    protected:
        IpAndPort ip_and_port;

        Player* player;

    private:
        Server& server;
};
