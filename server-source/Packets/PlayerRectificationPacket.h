#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class PlayerRectificationPacket : public ECCPacket
{
    public:
        PlayerRectificationPacket(sf::Vector2f new_pos) :
            ECCPacket(Networking::StoC::PlayerRectification)
        {
            operator<<(new_pos.x);
            operator<<(new_pos.y);
        }

    private:
};
