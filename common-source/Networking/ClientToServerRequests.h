#pragma once

#include "IpAndPort.h"

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
    std::string nickname;
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
    int type;
    union
    {
        struct
        {
            float walk_mov_x, walk_mov_y;
            float walk_pos_x, walk_pos_y;
        };
        struct
        {
            int item_pos_x, item_pos_y;
            uint32_t item_in_hand;
        };
        sf::Vector2i break_block_pos;
        struct
        {
            int item_swap_pos;
            uint32_t hand_item;
            uint32_t slot_item;
        };
        unsigned int entity_id;
    };
};


using CtoSRequestQueue = NetworkRequestQueue<DisconnectRequest, ConnectionRequest,
                                             ChunkRequest, EntityRequest,
                                             KeepAliveRequest, PlayerActionRequest>;

}
}
