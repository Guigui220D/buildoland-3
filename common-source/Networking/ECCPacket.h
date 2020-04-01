#ifndef ECCPACKET_H
#define ECCPACKET_H

#include <SFML/Network/Packet.hpp>

#include <vector>

class ECCPacket : public sf::Packet
{
public:
    virtual const void * onSend(std::size_t &size) override;
    virtual void onReceive(const void *data, std::size_t size) override;

    bool isCorrupted() const
    { return m_corrupted; }

private:
    std::vector<sf::Uint8> m_ecc_data;
    bool m_corrupted { false };
};

#endif // ECCPACKET_H
