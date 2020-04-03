#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class PlayerInventory;

class FullInventoryPacket : public ECCPacket
{
    public:
        FullInventoryPacket(PlayerInventory& inv);


    protected:

    private:
};
