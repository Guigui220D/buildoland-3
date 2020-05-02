#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
#include "../../../client-source/Utils/Animation.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#else
class Client;
    #include "../../../common-source/Networking/ClientToServerRequests.h"
    #include "../../../external/json/Json.hpp"
#endif // CLIENT_SIDE

#include "../../Items/PlayerInventory.h"

class Chunk;

class Player : public LivingEntity
{
public:
#ifdef CLIENT_SIDE
    static unsigned int this_player_id;
    static Player* this_player;
#endif // CLIENT_SIDE

    inline unsigned short getEntityCode() const override { return Entities::Player; };

#ifdef CLIENT_SIDE
    Player(World& world, unsigned int id);
#else
    Player(World& world, unsigned int id, const Client& client);
#endif // CLIENT_SIDE
    ~Player();

    void update(float delta);
#ifdef CLIENT_SIDE
    void draw(sf::RenderTarget& target) const;
    void drawAbove(sf::RenderTarget& target) const;
    int getZOrder() const override
    { return 1; }

    void useHand(sf::Vector2i pos);

    bool takeNewEntityPacket(ECCPacket& packet) override;
#else
    void handlePlayerActionRequest(const Networking::CtoS::PlayerActionRequest& rq);

    inline const Client& getClient() const { return client; }

    void makeNewEntityPacket(ECCPacket& packet) const override;
#endif

    inline void setPosition(sf::Vector2f new_pos) { position = new_pos; }

    bool isSubscribedTo(sf::Vector2i chunk, bool twice = false) const;

    inline PlayerInventory& getInventory() { return inventory; }

    #ifdef CLIENT_SIDE
    sf::Vector2f getVisualHitbox() const override
    { return sf::Vector2f(.5f, 1.0f); }
    #endif

    #ifndef CLIENT_SIDE
    virtual nlohmann::json* serializeToJson() const override;
    virtual void deserialize(nlohmann::json& json) override;
    #endif // CLIENT_SIDE

private:
    PlayerInventory inventory;

    std::array<uint32_t, 4> outfit_colors;

#ifdef CLIENT_SIDE
    void moreOnChunkChange(sf::Vector2i old_chunk, sf::Vector2i new_chunk) override;

    sf::RectangleShape base;
    sf::RectangleShape shoes;
    sf::RectangleShape pants;
    sf::RectangleShape shirt;
    sf::CircleShape shadow;

    sf::Text nickname_text;

    sf::Vector2f last_walking_direction;
    sf::Clock frequent_walk_update;

    bool autowalk = false;
    sf::Clock aw_break;
#else
    const Client& client;
#endif
};
