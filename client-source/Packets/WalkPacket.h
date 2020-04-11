#pragma once

#include "../../common-source/Networking/ECCPacket.h"

#include <SFML/System/Vector2.hpp>

class WalkPacket : public ECCPacket
{
    public:
        WalkPacket(sf::Vector2f dir, sf::Vector2f pos) :
            ECCPacket(Networking::CtoS::PlayerAction)
        {
            operator<<(EntityActions::CtoS::Walk);

            operator<<(dir.x);
            operator<<(dir.y);

            operator<<(pos.x);
            operator<<(pos.y);
        }

    private:
};
