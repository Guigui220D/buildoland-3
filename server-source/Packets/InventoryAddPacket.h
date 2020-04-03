#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class InventoryAddPacket : public ECCPacket
{
    public:
        InventoryAddPacket(uint32_t item) :
            ECCPacket(Networking::StoC::InventoryUpdate)
        {
            operator<<(InventoryUpdates::StoC::AddStack);

            operator<<(item);
        }

    private:
};
