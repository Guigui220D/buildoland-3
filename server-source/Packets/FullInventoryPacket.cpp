#include "FullInventoryPacket.h"

#include "../../common-source/Items/PlayerInventory.h"

FullInventoryPacket::FullInventoryPacket(PlayerInventory& inv) :
    ECCPacket(Networking::StoC::InventoryUpdate)
{
    operator<<(InventoryUpdates::StoC::SetInventory);

    for (int i = 0; i < 25; i++)
        operator<<(inv.contents.at(i).getInt());
}
