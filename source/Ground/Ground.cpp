#include "Ground.h"

#include "../World/World.h"
#include "../Game.h"

#include "../Utils/XXHash.hpp"
#include "../Utils/XXHash_bis.hpp"

const TilesetHelper<16, 16, 1> Ground::tilesetHelper;
const TilesetHelper<16, 8, 1> Ground::tilesetHelperDetails;

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

sf::VertexArray Ground::getSurfaceDetails(GroundInfo info, int frame) const
{
    sf::VertexArray bleedings(sf::Quads);
    addNeighborsBleeding(info, bleedings, frame);
    return bleedings;
}

bool Ground::acceptsTextureBleedings(GroundInfo info, const Ground* other) const
{
    return (other == info.getWorld()->getGame()->getGroundsManager().WATER && other->getId() != getId()) || (other->hasTextureBleedings() && (other->getId() > id));
}

void Ground::addNeighborsBleeding(GroundInfo info, sf::VertexArray& vertex_array, int frame) const
{
    for (int i = 0; i < 4; i++)
    {
        sf::Vector2i opos = info.getPos() + utils::getRelativeBlock(i);
        const Ground* other_ground = info.getWorld()->getGround(opos, false);
        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(GroundInfo(info.getWorld(), opos), frame), i);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
}
