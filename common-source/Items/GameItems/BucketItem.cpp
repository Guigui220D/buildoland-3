#include "BucketItem.h"

#include "../ItemStack.h"

#ifndef CLIENT_SIDE
    #include "../../../server-source/Server/ClientsManager.h"
    #include "../../../server-source/Packets/InventorySetPacket.h"
    #include "../../Entities/GameEntities/Player.h"
    #include "../../../server-source/World/World.h"
    #include "../../Grounds/Ground.h"
    #include "../../Grounds/GameGrounds.h"
    #include "../ItemsRegister.h"
#endif // CLIENT_SIDE

EmptyBucketItem::EmptyBucketItem() :
    Item("empty_bucket", 4)
{

}

EmptyBucketItem::~EmptyBucketItem()
{
    //dtor
}

void EmptyBucketItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
#ifndef CLIENT_SIDE
    TileReference tr = world.getTile(click_pos);

    if (tr.getGround() == GameGrounds::WATER)
    {
        ItemStack water_bucket(ItemsRegister::WATER_BUCKET);

        InventorySetPacket set(0, water_bucket.getInt());
        player.getClient().send(set);

        stack.swap(water_bucket);

        tr.setGround(GameGrounds::SAND);
    }
#endif // CLIENT_SIDE
}

WaterBucketItem::WaterBucketItem() :
    Item("water_bucket", 5)
{

}

WaterBucketItem::~WaterBucketItem()
{
    //dtor
}

void WaterBucketItem::use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const
{
#ifndef CLIENT_SIDE
    TileReference tr = world.getTile(click_pos);

    if (tr.getGround() == GameGrounds::SAND)
    {
        ItemStack empty_bucket(ItemsRegister::EMPTY_BUCKET);

        InventorySetPacket set(0, empty_bucket.getInt());
        player.getClient().send(set);

        stack.swap(empty_bucket);

        tr.setGround(GameGrounds::WATER);
    }
#endif // CLIENT_SIDE
}
