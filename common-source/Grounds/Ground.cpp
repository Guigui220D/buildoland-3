#include "Ground.h"

#ifdef CLIENT_SIDE
    #include "../../external/xxhash/XXHash.hpp"
    #include "../../external/xxhash/XXHash_bis.hpp"

    #include "../../client-source/World/World.h"
    #include "../../client-source/Game.h"
    #include "../../common-source/Grounds/GameGrounds.h"
#else
    #include "../Items/ItemsRegister.h"
#endif

Ground::~Ground()
{
    //dtor
}

#ifndef CLIENT_SIDE
Ground::Ground(const std::string name, bool should_have_item, uint32_t default_texture) :
    name(name),
    has_item(should_have_item),
    drop(ItemsRegister::NULL_ITEM)
{
    //ctor
}

std::vector<ItemStack> Ground::getDrops() const
{
    if (!has_item)
        return {};
    std::vector<ItemStack> vec;
    vec.emplace_back(drop);
    return vec;
}

#else
const TilesetHelper<16, 16, 1> Ground::tilesetHelper;
const TilesetHelper<16, 8, 1> Ground::tilesetHelperDetails;

Ground::Ground(const std::string name, bool should_have_item, uint32_t default_texture) :
    name(name),
    has_item(should_have_item),
    default_texture(default_texture)
{
    //ctor
}

uint32_t Ground::getRandomInt(TileReference info, int add)
{
    //Pseudo random from seed and ground position
    sf::Vector2i pos = info.getPos();
    return XXH32(&pos, sizeof(pos), info.getWorld().getSeed() + add);
}

sf::VertexArray Ground::getSurfaceDetails(TileReference info, int frame) const
{
    sf::VertexArray bleedings(sf::Quads);
    addNeighborsBleeding(info, bleedings, frame);
    return bleedings;
}

bool Ground::acceptsTextureBleedings(TileReference info, const Ground* other) const
{
    return (other == GameGrounds::WATER && other->getId() != getId()) || (other->hasTextureBleedings() && (other->getId() > getId()));
}

void Ground::addNeighborsBleeding(TileReference info, sf::VertexArray& vertex_array, int frame) const
{
    for (int i = 0; i < 4; i++)
    {
        sf::Vector2i opos = info.getPos() + utils::getRelativeBlock(i);
        const Ground* other_ground = info.getWorld().getGroundPtr(opos);

        if (acceptsTextureBleedings(info, other_ground))
        {
            Quad tex = tilesetHelperDetails.getFourVertices(other_ground->getBleedingForNeighborGrounds(TileReference(opos, info.getWorld()), frame), i);
            TextQuad quad = utils::getSquare(tex, info.getPos());
            for (int i = 0; i < 4; i++)
                vertex_array.append(quad.verts[i]);
        }
    }
}
#endif
