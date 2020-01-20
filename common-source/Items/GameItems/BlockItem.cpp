#include "BlockItem.h"

#include "../ItemStack.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/World/World.h"
#endif // CLIENT_SIDE

BlockItem::BlockItem(Block const * block) :
    Item(block->getName() + "_block"),
    block(block)
{
    //ctor
}

BlockItem::~BlockItem()
{
    //dtor
}

#ifndef CLIENT_SIDE
void BlockItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos) const
{
    if (world.getBlock(click_pos) == GameBlocks::AIR)
    {
        stack.takeSome(1);
        world.setBlock(click_pos, block->getId());
    }
}
#endif // CLIENT_SIDE

