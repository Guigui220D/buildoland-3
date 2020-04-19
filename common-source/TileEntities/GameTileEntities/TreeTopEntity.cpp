#include "TreeTopEntity.h"

/*

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
    #include "../../../client-source/Res/ResourceManager.h"
    #include <SFML/Graphics/RenderTarget.hpp>
#endif // CLIENT_SIDE

TreeTopEntity::TreeTopEntity(World& world, unsigned int id, sf::Vector2i tile_pos) :
    TileEntity(world, id, tile_pos)
{
    #ifdef CLIENT_SIDE
    rectangle.setPosition(getPosition());
    rectangle.setSize(sf::Vector2f(3.f, 3.f));
    rectangle.setOrigin(sf::Vector2f(1.5f, 3.f));
    rectangle.setTexture(&world.getGame().getResourceManager().getTexture("TREE_TOP"));
    //rectangle.setFillColor(sf::Color::Yellow);
    #endif // CLIENT_SIDE
}

TreeTopEntity::~TreeTopEntity()
{
    //dtor
}

#ifdef CLIENT_SIDE
void TreeTopEntity::drawAbove(sf::RenderTarget& target) const
{
    if (!isReady())
        return;

    target.draw(rectangle);
}
#endif
*/
