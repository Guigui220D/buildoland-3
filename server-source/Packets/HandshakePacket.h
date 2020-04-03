#pragma once

#include "../../common-source/Networking/ECCPacket.h"

#include "../Version.h"

class HandshakePacket : public ECCPacket
{
    public:
        HandshakePacket(int seed, unsigned int player_id) :
            ECCPacket(Networking::StoC::FinalHandshake)
        {
            operator<<(Version::VERSION_SHORT);
            operator<<(seed);
            operator<<(player_id);
        }

    private:
};
