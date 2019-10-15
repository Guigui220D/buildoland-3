#include "ClientsManager.h"

#include "Server.h"

ClientsManager::ClientsManager(Server* server) :
    server(server)
{
    //ctor
}

ClientsManager::~ClientsManager()
{
    //dtor
}

bool ClientsManager::addClient(IpAndPort& client, Player* player)
{
    sf::Lock l(clients_mutex);

    if (clients.find(client) != clients.cend())
        return false;

    clients.emplace(std::pair<IpAndPort, std::unique_ptr<Client>>(client, std::make_unique<Client>(server, client, player)));
    return true;
}

Client& ClientsManager::getClient(IpAndPort& client) const
{
    sf::Lock l(clients_mutex);

    auto i = clients.find(client);

    if (i == clients.cend())
        throw new std::logic_error("getClient: client doesn't exist.");

    return *i->second;
}

void ClientsManager::sendToAll(sf::Packet& packet)
{
    for (auto i = clients.begin(); i != clients.end(); i++)
        i->second->send(packet);
}
