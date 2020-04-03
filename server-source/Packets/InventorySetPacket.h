#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class InventorySetPacket : public ECCPacket
{
    public:
        InventorySetPacket(int slot, uint32_t item) :
            ECCPacket(Networking::StoC::InventoryUpdate)
        {
            operator<<(InventoryUpdates::StoC::SetStack);

            operator<<(slot);
            operator<<(item);
        }

    private:
};
