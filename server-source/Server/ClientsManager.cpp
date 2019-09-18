#include "ClientsManager.h"

ClientsManager::ClientsManager()
{
    //ctor
}

ClientsManager::~ClientsManager()
{
    //dtor
}

bool ClientsManager::addClient(IpAndPort client)
{
    if (isConnected(client))
        return false;

    clients.emplace(std::pair<IpAndPort, Client>(client, Client(client)));
    return true;
}
