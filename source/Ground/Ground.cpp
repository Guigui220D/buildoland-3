#include "Ground.h"

#include "../World/World.h"
#include "../Game.h"

#include "../Utils/XXHash.hpp"
#include "../Utils/XXHash_bis.hpp"

const TilesetHelper<16, 16, 0> Ground::tilesetHelper;
const TilesetHelper<16, 8, 0> Ground::tilesetHelperDetails;

Ground::Ground(const std::string name, uint32_t default_texture) :
    name(name),
    default_texture(default_texture)
{
    //ctor
}

Ground::~Ground()
{
    //dtor
}

uint32_t Ground::getRandomInt(GroundInfo info)
{
    //Pseudo random from seed and ground position
    sf::Vector2i pos = info.getPos();
    return XXH32(&pos, sizeof(pos), info.getWorld()->getSeed());
}

sf::VertexArray Ground::getSurfaceDetails(GroundInfo info) const
{
    sf::VertexArray bleedings(sf::Quads);
    addNeighborsBleeding(info, bleedings);
    return bleedings;
}

bool Ground::acceptsTextureBleedings(GroundInfo info, const Ground* other) const
{
    return other->hasTextureBleedings() && (other->getId() > id);
}

void Ground::addNeighborsBleeding(GroundInfo info, sf::VertexArray& vertex_array) const
{
    //Up
    {
        sf::Vector2i opos = info.getPos() + sf::Vector2i(0, -1);
        uint16_t oid = info.getWorld()->getGround(opos, false);
        //TO CHANGE
            const Ground* other_ground = info.getWorld()->getGame()->getGroundsManager().getGroundByID(oid);
        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(GroundInfo(info.getWorld(), opos)), 0);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
    //Right
    {
        sf::Vector2i opos = info.getPos() + sf::Vector2i(1, 0);
        uint16_t oid = info.getWorld()->getGround(opos, false);
        //TO CHANGE
            const Ground* other_ground = info.getWorld()->getGame()->getGroundsManager().getGroundByID(oid);
        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(GroundInfo(info.getWorld(), opos)), 1);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
    //Down
    {
        sf::Vector2i opos = info.getPos() + sf::Vector2i(0, 1);
        uint16_t oid = info.getWorld()->getGround(opos, false);
        //TO CHANGE
            const Ground* other_ground = info.getWorld()->getGame()->getGroundsManager().getGroundByID(oid);
        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(GroundInfo(info.getWorld(), opos)), 2);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
    //Left
    {
        sf::Vector2i opos = info.getPos() + sf::Vector2i(-1, 0);
        uint16_t oid = info.getWorld()->getGround(opos, false);
        //TO CHANGE
            const Ground* other_ground = info.getWorld()->getGame()->getGroundsManager().getGroundByID(oid);
        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(GroundInfo(info.getWorld(), opos)), 3);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
}
