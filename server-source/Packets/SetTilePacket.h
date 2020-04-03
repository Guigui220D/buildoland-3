#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class SetTilePacket : public ECCPacket
{
    public:
        SetTilePacket(bool ground, uint16_t tile_id, sf::Vector2i pos) :
            ECCPacket()
        {
            if (ground)
                operator<<(Networking::StoC::GroundUpdate);
            else
                operator<<(Networking::StoC::BlockUpdate);

            operator<<(pos.x);
            operator<<(pos.y);

            operator<<(tile_id);
        }

    private:
};
