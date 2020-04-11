#pragma once

#include <string>

#include <SFML/Network/Socket.hpp>

class utils
{
    public:

        static std::string statusToString(sf::Socket::Status status)
        {
            switch (status)
            {
            case sf::Socket::Done:
                return "DONE";
            case sf::Socket::NotReady:
                return "NOT READY";
            case sf::Socket::Partial:
                return "PARTIAL";
            case sf::Socket::Disconnected:
                return "DISCONNECTED";
            case sf::Socket::Error:
                return "ERROR";

            default:
                return "???";
            }
        }

        static constexpr uint64_t combine(int32_t a, int32_t b)
        {
            uint64_t lon = a;
            lon <<= 32;
            lon |= 0xFFFFFFFF & b;
            return lon;
        }
};
