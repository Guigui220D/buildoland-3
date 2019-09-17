#pragma once

#include <string>

namespace Utils
{
    std::string statusToString(sf::Socket::Status status)
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
}
