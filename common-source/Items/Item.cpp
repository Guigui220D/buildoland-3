#include "Item.h"

#include "ItemStack.h"

#include "../Networking/NetworkingCodes.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/States/GameState.h"
    #include "../../client-source/World/World.h"
    #include "../../client-source/Packets/ItemUsePacket.h"
#endif // CLIENT_SIDE

#ifdef CLIENT_SIDE
const TilesetHelper<16, 16, 1> Item::tilesetHelper;

Item::Item(const std::string& name, uint32_t default_texture) :
    name(name),
    default_texture(default_texture)
{
    //ctor
}
#else
Item::Item(const std::string &name, uint32_t default_texture) :
    name(name)
{
    //ctor
}
#endif // CLIENT_SIDE


Item::~Item()
{
    //dtor
}

void Item::useItem(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
    if (!stack)
    {
        stack.reset();
        return;
    }

    #ifdef CLIENT_SIDE
    ItemUsePacket use_packet(click_pos, stack.getInt());
    world.getState().sendToServer(use_packet);
    #endif

    stack.getItem()->use(stack, world, click_pos, player);
}

void Item::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{}
