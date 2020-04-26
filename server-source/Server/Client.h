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
        Client(Server& server, IpAndPort ip_and_port, const std::string& in_nickname, Player* player);
        virtual ~Client();

        inline const IpAndPort& getIpAndPort() const { return ip_and_port; }

        inline bool hasPlayer() const { return player; }
        inline Player* getPlayer() const { return player; }
        inline void setPlayer(Player* new_player) { player =  new_player; }

        inline std::string getNickname() const
        { return nickname; }

        void send(ECCPacket& packet) const;

        inline bool operator==(const Client& other) const
        {
            return ip_and_port == other.ip_and_port;
        }

        sf::Clock last_packet_received;
        bool admin = false;

    protected:
        IpAndPort ip_and_port;
        std::string nickname;

        Player* player;

    private:
        Server& server;
};
