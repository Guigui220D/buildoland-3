#include "TileEntity.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/Chunk.h"
#else
    #include "../../server-source/World/Chunk.h"
#endif // CLIENT_SIDE

TileEntity::TileEntity(World& world, unsigned int id, Chunk& chunk, sf::Vector2i tile_pos) :
    Entity(world, id),
    chunk(chunk),
    tile_pos(tile_pos)
{
    position = sf::Vector2f(.5f + tile_pos.x, .5f + tile_pos.y);
    chunk_on = chunk.getPos();
}

TileEntity::~TileEntity()
{
    //dtor
}

#ifndef CLIENT_SIDE
void TileEntity::makeNewEntityPacket(sf::Packet& packet) const
{
    packet.clear();

    packet << Networking::StoC::EntityAction;
    packet << EntityActions::StoC::AddEntity;
    packet << getEntityCode(); //This is tile entity
    packet << getId();
    packet << getTileEntityCode();
    packet << tile_pos.x << tile_pos.y;

    addInfoToNewEntityPacket(packet);
}
#endif
