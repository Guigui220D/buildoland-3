#include "ClientsManager.h"

#include "Server.h"

#include <iostream>

ClientsManager::ClientsManager(Server& server) :
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

    if (clients.find(client) != clients.end())
        return false;

    clients.emplace(std::pair<IpAndPort, std::unique_ptr<Client>>(client, std::make_unique<Client>(server, client, player)));

    return true;
}

void ClientsManager::removeClient(IpAndPort& client)
{
    sf::Lock l(clients_mutex);

    auto i = clients.find(client);

    if (i == clients.cend())
        return;

    assert(!i->second->hasPlayer());

    clients.erase(i);
}

Client& ClientsManager::getClient(IpAndPort& client) const
{
    sf::Lock l(clients_mutex);

    auto i = clients.find(client);

    if (i == clients.end())
        throw new std::logic_error("getClient: client doesn't exist.");

    return *i->second;
}

void ClientsManager::resetClientTimer(IpAndPort& client)
{
    sf::Lock l(clients_mutex);

    if (clients.find(client) == clients.end())
        return;

    getClient(client).last_packet_received.restart();
}

int ClientsManager::doTimeOuts(float timeout_s)
{
    int yeet_count = 0;

    sf::Lock l(clients_mutex);

    for (auto i = clients.begin(); i != clients.end(); )
    {
        if (i->second->last_packet_received.getElapsedTime().asSeconds() > timeout_s)
        {
            #ifdef SOLO
            if (i->first == server->owner)
            {
                server->running = false;
                server->passReceiveOnce();
                break;
            }
            #endif // SOLO
            std::cout << "Disconnecting client " << i->first.address << ':' << i->first.port << " (timeout)." << std::endl;

            Client& client = *(i->second);

            if (client.hasPlayer())
            {
                server.getWorld().getEntityManager().removeEntity(client.getPlayer()->getId());
                client.setPlayer(nullptr);
            }

            i = clients.erase(i);
            yeet_count++;

            std::cout << "Client disconnected." << std::endl;
        }
        else
            i++;
    }

    return yeet_count;
}

void ClientsManager::sendToAll(ECCPacket& packet)
{
    sf::Lock l(clients_mutex);

    for (auto i = clients.begin(); i != clients.end(); i++)
        i->second->send(packet);
}
