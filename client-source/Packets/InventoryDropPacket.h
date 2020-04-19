#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class InventoryDropPacket : public ECCPacket
{
public:
    InventoryDropPacket(uint32_t hand_item) :
          ECCPacket(Networking::CtoS::PlayerAction)
    {
        operator<<(EntityActions::CtoS::DropInventoryItem);

        operator<<(hand_item);
    }

private:
};
