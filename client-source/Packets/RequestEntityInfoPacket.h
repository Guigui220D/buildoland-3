#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class RequestEntityInfoPacket : public ECCPacket
{
    public:
        RequestEntityInfoPacket(unsigned int entity_id) :
            ECCPacket(Networking::CtoS::RequestEntityInfo)
        {
            operator<<(entity_id);
        }

    private:
};
