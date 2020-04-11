#pragma once

#include "../../common-source/Networking/ECCPacket.h"

#include <SFML/System/Vector2.hpp>

class BreakBlockPacket : public ECCPacket
{
    public:
        BreakBlockPacket(sf::Vector2i block_pos) :
            ECCPacket(Networking::CtoS::PlayerAction)
        {
            operator<<(EntityActions::CtoS::BreakBlock);

            operator<<(block_pos.x);
            operator<<(block_pos.y);
        }

    private:
};
