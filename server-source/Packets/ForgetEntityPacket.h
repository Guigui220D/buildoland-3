#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class ForgetEntityPacket : public ECCPacket
{
    public:
        ForgetEntityPacket(unsigned int entity_id) :
            ECCPacket(Networking::StoC::EntityAction)
        {
            operator<<(EntityActions::StoC::ForgetEntity);

            operator<<(entity_id);
        }

    private:
};
