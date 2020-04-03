#pragma once

#include <SFML/Network/Packet.hpp>

#include <vector>

#ifdef CLIENT_SIDE
    #include "ClientToServerCodes.h"
    #include "CtoS_PlayerActionCodes.h"
#else
    #include "ServerToClientCodes.h"
#endif // CLIENT_SIDE

class ECCPacket : public sf::Packet
{
public:
    virtual const void * onSend(std::size_t &size) override;
    virtual void onReceive(const void *data, std::size_t size) override;

    ECCPacket() {};
    ECCPacket(int code) { operator<<(code); };

    bool isCorrupted() const
    { return m_corrupted; }

private:
    std::vector<sf::Uint8> m_ecc_data;
    bool m_corrupted { false };
};
