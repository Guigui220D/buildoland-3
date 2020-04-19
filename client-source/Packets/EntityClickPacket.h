#ifndef ENTITYCLICKPACKET_H
#define ENTITYCLICKPACKET_H

#include "../../common-source/Networking/ECCPacket.h"

#include <SFML/System/Vector2.hpp>

class EntityClickPacket : public ECCPacket
{
public:
    EntityClickPacket(unsigned int entity_id, int click_type) :
          ECCPacket(Networking::CtoS::PlayerAction)
    {
        operator<<(click_type);

        operator<<(entity_id);
    }

private:
};


#endif // ENTITYCLICKPACKET_H
