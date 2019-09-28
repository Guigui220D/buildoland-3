#pragma once

#include <map>
#include <memory>

#include "Client.h"

class Server;

class ClientsManager
{
    public:
        ClientsManager(Server* server);
        ~ClientsManager();

        inline bool isConnected(IpAndPort client) const { sf::Lock l(clients_mutex); return clients.find(client) != clients.cend(); }

        bool addClient(IpAndPort& client);
        Client& getClient(IpAndPort& client) const;

        void sendToAll(sf::Packet& packet);

        mutable sf::Mutex clients_mutex;

        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsBegin() const { return clients.cbegin(); }
        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsEnd() const { return clients.cend(); }

    private:
        Server* const server;

        std::map<IpAndPort, std::unique_ptr<Client>> clients;
};
