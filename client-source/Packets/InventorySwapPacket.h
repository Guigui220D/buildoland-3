#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class InventorySwapPacket : public ECCPacket
{
    public:
        InventorySwapPacket(int slot) :
            ECCPacket(Networking::CtoS::PlayerAction)
        {
            operator<<(EntityActions::CtoS::SwapInventoryItem);

            operator<<(slot);
        }

    private:
};
