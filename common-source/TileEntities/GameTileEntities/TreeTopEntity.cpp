#include "TreeTopEntity.h"

#ifdef CLIENT_SIDE
    #include "../../../client-source/Game.h"
    #include "../../../client-source/World/World.h"
    #include "../../../client-source/Res/ResourceManager.h"
    #include <SFML/Graphics/RenderTarget.hpp>
#endif // CLIENT_SIDE

TreeTopEntity::TreeTopEntity(Chunk& chunk, sf::Vector2i pos) :
    TileEntity(chunk, pos)
{
    #ifdef CLIENT_SIDE
    rectangle.setPosition(sf::Vector2f(getTilePos().x, getTilePos().y));
    rectangle.setSize(sf::Vector2f(3.f, 3.f));
    rectangle.setOrigin(sf::Vector2f(1.5f, 3.f));
    rectangle.setTexture(&getWorld().getGame().getResourceManager().getTexture("TREE_TOP"));
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
    target.draw(rectangle);
}
#endif
