#include "TileEntity.h"

#ifdef CLIENT_SIDE
    #include <SFML/Graphics/RenderTarget.hpp>
    #include "../../client-source/World/World.h"
#else
    #include "../../server-source/World/World.h"
    #include "../../server-source/World/Chunk.h"

    #include "../../external/json/Json.hpp"
    #include <assert.h>
#endif

TileEntity::TileEntity(Chunk& chunk, sf::Vector2i pos) :
    world(chunk.getWorld()),
    chunk(chunk),
    tile_pos(pos)
{
    //ctor
}

TileEntity::~TileEntity()
{
    //dtor
}

sf::Vector2i TileEntity::getPosInChunk() const
{
    return World::getBlockPosInChunk(tile_pos);
}

void TileEntity::update(float delta) {}

#ifdef CLIENT_SIDE
void TileEntity::draw(sf::RenderTarget& target) const {}
void TileEntity::drawAbove(sf::RenderTarget& target) const {}
bool TileEntity::readTileEntityPacket(ECCPacket& packet) { return true; }
#else
nlohmann::json* TileEntity::serializeToJson() const
{
    nlohmann::json* json = new nlohmann::json();
    (*json)["type"] = getTypeCode();
    return json;
}

void TileEntity::deserialize(nlohmann::json& json)
{
}
#endif

