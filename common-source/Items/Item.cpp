#include "Item.h"

#include "ItemStack.h"

#include "../../client-source/World/World.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/States/GameState.h"
#endif // CLIENT_SIDE

#ifdef CLIENT_SIDE
const TilesetHelper<16, 16, 1> Item::tilesetHelper;

Item::Item(const std::string name, uint32_t default_texture) :
    name(name),
    default_texture(default_texture)
{
    //ctor
}
#else
Item::Item(const std::string name, uint32_t default_texture) :
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
    ECCPacket use_packet;

    use_packet << Networking::CtoS::PlayerAction;
    use_packet << EntityActions::CtoS::UseItem;
    use_packet << click_pos.x << click_pos.y;

    world.getState().sendToServer(use_packet);
    #endif

    stack.getItem()->use(stack, world, click_pos, player);
}

void Item::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{}
