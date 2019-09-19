#pragma once

#include <map>
#include <memory>

#include "Client.h"

class ClientsManager
{
    public:
        ClientsManager();
        ~ClientsManager();

        inline bool isConnected(IpAndPort client) const { sf::Lock l(clients_mutex); return clients.find(client) != clients.cend(); }

        bool addClient(IpAndPort& client);
        Client& getClient(IpAndPort& client) const;

        mutable sf::Mutex clients_mutex;

        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsBegin() const { return clients.cbegin(); }
        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsEnd() const { return clients.cend(); }

    private:
        std::map<IpAndPort, std::unique_ptr<Client>> clients;
};
