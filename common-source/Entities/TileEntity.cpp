#include "TileEntity.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/World/Chunk.h"
    #include "../../client-source/World/World.h"

    #include "GameEntities/Player.h"
#else
    #include "../../server-source/World/Chunk.h"
    #include "../../server-source/World/World.h"

    #include <iostream>
#endif // CLIENT_SIDE

TileEntity::TileEntity(World& world, unsigned int id, sf::Vector2i tile_pos) :
    Entity(world, id),
    chunk(nullptr),
    tile_pos(tile_pos),
    chunk_pos(World::getChunkPosFromBlockPos(tile_pos)),
    ready(false)
{
    position = sf::Vector2f(std::round(tile_pos.x), std::round(tile_pos.y));
    chunk_on = chunk_pos;
}

TileEntity::~TileEntity()
{
}

void TileEntity::assignChunk(Chunk* chunk_ptr)
{
    sf::Vector2i pos = World::getBlockPosInChunk(tile_pos);

    chunk = chunk_ptr;

    if (chunk_ptr)
    {
        chunk->tile_entities[pos.y*Chunk::CHUNK_SIZE + pos.x] = this;
        ready = true;
    }
    else
    {
        to_be_removed = true;
        ready = false;
    }
}

void TileEntity::update(float delta)
{
    #ifdef CLIENT_SIDE
    if (!ready)
    {
        if (!Player::this_player || Player::this_player->isSubscribedTo(chunk_pos))
            to_be_removed = true;
    }
    #endif // CLIENT_SIDE

    if (ready)
        teUpdate(delta);
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

void TileEntity::teUpdate(float delta) {}
