#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class ItemUsePacket : public ECCPacket
{
    public:
        ItemUsePacket(sf::Vector2i click_pos, uint32_t item_in_hand) :
            ECCPacket(Networking::CtoS::PlayerAction)
        {
            operator<<(EntityActions::CtoS::UseItem);

            operator<<(click_pos.x);
            operator<<(click_pos.y);

            operator<<(item_in_hand);
        }

    private:
};
