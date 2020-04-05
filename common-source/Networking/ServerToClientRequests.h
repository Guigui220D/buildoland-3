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
    sf::Vector2i pos;
    std::vector<uint8_t> chunk_data;
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
    int type;
    union
    {
        uint32_t stack_add;
        struct
        {
            uint32_t stack_set;
            unsigned int pos;
        };
        uint32_t stack_list[25];
    };
    ECCPacket data_packet;
};

using StoCRequestQueue = NetworkRequestQueue<DisconnectRequest, SendChunkRequest,
                                             EntityActionRequest, BlockUpdateRequest,
                                             GroundUpdateRequest, PlayerRectificationRequest,
                                             InventoryUpdateRequest>;

}
}
