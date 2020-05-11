#include "TestTileEntity.h"

#include "../../Utils/Log.h"

#include <cstdlib>

#ifdef CLIENT_SIDE
    #include <SFML/Graphics/RenderTarget.hpp>
#else
    #include "../../../server-source/World/World.h"
    #include "../../../server-source/World/Chunk.h"

    #include "../../../external/json/Json.hpp"
#endif // CLIENT_SIDE

#include "../../Networking/ECCPacket.h"

TestTileEntity::TestTileEntity(Chunk& chunk, sf::Vector2i pos) :
    TileEntity(chunk, pos)
{
    log(INFO, "Test_te : Hello! from {}; {}.\n", getTilePos().x, getTilePos().y);

    #ifdef CLIENT_SIDE
    cs.setPosition(sf::Vector2f(getTilePos().x, getTilePos().y));
    cs.setRadius(1.f);
    cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()));
    cs.setFillColor(sf::Color::Transparent);
    cs.setOutlineColor(sf::Color::Yellow);
    cs.setOutlineThickness(0.1f);
    //rectangle.setFillColor(sf::Color::Yellow);
    #endif // CLIENT_SIDE
}

TestTileEntity::~TestTileEntity()
{
    log(INFO, "Test_te : Goodbye. from {}; {}.\n", getTilePos().x, getTilePos().y);
}

void TestTileEntity::update(float delta)
{
    #ifndef CLIENT_SIDE

    if (clk.getElapsedTime().asSeconds() > 1.f)
    {
        clk.restart();

        ECCPacket to_client;
        to_client << Networking::StoC::TileEntityUpdate;
        to_client << getTilePos().x << getTilePos().y;

        value = std::rand() % 5;
        to_client << value;

        getWorld().sendToSubscribers(to_client, getChunk().getPos());
    }
    #endif
}

#ifdef CLIENT_SIDE
void TestTileEntity::draw(sf::RenderTarget& target) const
{
    target.draw(cs);
}

bool TestTileEntity::readTileEntityPacket(ECCPacket& packet)
{
    log(INFO, "Test_te : Received packet. from {}; {}.\n", getTilePos().x, getTilePos().y);

    int new_size; packet >> new_size;

    cs.setRadius(new_size);
    cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()));

    return true;
}
#else
nlohmann::json* TestTileEntity::serializeToJson() const
{
    nlohmann::json* json = new nlohmann::json();
    (*json)["type"] = getTypeCode();
    (*json)["clk"] = clk.getElapsedTime().asSeconds();
    (*json)["value"] = value;
    return json;
}

void TestTileEntity::deserialize(nlohmann::json& json)
{
    /*
    if (json["clk"].is_number())
        json["clk"].get<float>();
        */

}
#endif
