#pragma once

#include "ECCPacket.h"

#include <SFML/System/Vector2.hpp>

template <typename... RequestTypes>
class NetworkRequestQueue;

namespace Networking
{
namespace StoC
{

struct DisconnectRequest
{
    ;
};

struct SendChunkRequest
{
    ECCPacket data_packet;
};

struct EntityActionRequest
{
    ECCPacket data_packet;
};

struct BlockUpdateRequest
{
    sf::Vector2i pos;
    uint16_t id;
};

struct GroundUpdateRequest
{
    sf::Vector2i pos;
    uint16_t id;
};

struct PlayerRectificationRequest
{
    sf::Vector2f pos;
};

struct InventoryUpdateRequest
{
    ECCPacket data_packet;
};

using StoCRequestQueue = NetworkRequestQueue<DisconnectRequest, SendChunkRequest,
                                             EntityActionRequest, BlockUpdateRequest,
                                             GroundUpdateRequest, PlayerRectificationRequest,
                                             InventoryUpdateRequest>;

}
}
