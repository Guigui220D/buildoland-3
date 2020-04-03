#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class InventorySwapPacket : public ECCPacket
{
    public:
        InventorySwapPacket(int slot, uint32_t hand_item, uint32_t slot_item) :
            ECCPacket(Networking::CtoS::PlayerAction)
        {
            operator<<(EntityActions::CtoS::SwapInventoryItem);

            operator<<(slot);

            operator<<(hand_item);
            operator<<(slot_item);
        }

    private:
};
