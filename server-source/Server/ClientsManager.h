#pragma once

#include <map>

#include "Client.h"

class ClientsManager
{
    public:
        ClientsManager();
        ~ClientsManager();

        inline bool isConnected(IpAndPort client) const { return clients.find(client) != clients.cend(); }

        bool addClient(IpAndPort client);

    private:
        std::map<IpAndPort, Client> clients;
};
