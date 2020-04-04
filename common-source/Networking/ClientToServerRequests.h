#pragma once

#include "ECCPacket.h"

#include "../../server-source/Server/Client.h"

#include <SFML/System/Vector2.hpp>

template <typename... RequestTypes>
class NetworkRequestQueue;

namespace Networking
{
namespace CtoS
{

struct KeepAliveRequest
{
    IpAndPort iandp;
};

struct DisconnectRequest
{
    IpAndPort iandp;
};

struct ConnectionRequest
{
    IpAndPort iandp;
};

struct ChunkRequest
{
    IpAndPort iandp;
    sf::Vector2i pos;
};

struct EntityRequest
{
    IpAndPort iandp;
    unsigned int id;
};

struct PlayerActionRequest
{
    IpAndPort iandp;
    ECCPacket data_packet;
};


using CtoSRequestQueue = NetworkRequestQueue<DisconnectRequest, ConnectionRequest,
                                             ChunkRequest, EntityRequest,
                                             KeepAliveRequest, PlayerActionRequest>;

}
}
