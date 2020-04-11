#pragma once

#include <SFML/Network/IpAddress.hpp>

struct IpAndPort
{
    inline IpAndPort(sf::IpAddress ip, uint16_t port) :
          address(ip),
          port(port)
    {}

    inline IpAndPort(const IpAndPort& other) :
          address(other.address),
          port(other.port)
    {}

    const sf::IpAddress address;
    const uint16_t port;

    inline bool operator==(const IpAndPort& other) const
    {
        return ((port == other.port) && (address == other.address));
    }

    inline bool operator<(const IpAndPort& other) const
    {
        //For using IpAndPort as a map key
        return (address < other.address) ?
                                         true :
                                         (address == other.address ?
                                                                   port < other.port :
                                                                   false);
    }
};
