#include "ShovelItem.h"

#include "../../../common-source/Grounds/Ground.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/World/World.h"
    #include "../../Entities/GameEntities/Player.h"
    #include "../../Blocks/GameBlocks.h"
    #include "../../Grounds/GameGrounds.h"
#endif // CLIENT_SIDE


ShovelItem::ShovelItem() :
    Item("shovel", 2)
{

}

ShovelItem::~ShovelItem()
{
    //dtor
}

void ShovelItem::use(ItemStack& , World& world, sf::Vector2i click_pos, Player& player) const
{
    #ifndef CLIENT_SIDE
    TileReference tr = world.getTile(click_pos);

    if (tr.getGround()->isShovelable(tr)
        && tr.getBlock() == GameBlocks::AIR)
    {
        auto drops = tr.getGround()->getDrops();

        for (ItemStack& stack : drops)
            player.getInventory().insertItemStack(stack);

        tr.setGround(GameGrounds::DIRT);
    }
    #endif // CLIENT_SIDE
}
