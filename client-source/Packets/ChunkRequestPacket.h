#pragma once

#include "../../common-source/Networking/ECCPacket.h"

class ChunkRequestPacket : public ECCPacket
{
    public:
        ChunkRequestPacket(sf::Vector2i chunk_pos) :
            ECCPacket(Networking::CtoS::RequestChunk)
        {
            operator<<(chunk_pos.x);
            operator<<(chunk_pos.y);
        }

    private:
};
