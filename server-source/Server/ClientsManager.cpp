#include "ClientsManager.h"

#include "Server.h"
#include "../../server-source/World/World.h"
#include "../../server-source/World/EntitiesManager.h"
#include "../../common-source/Entities/GameEntities/Player.h"
#include "../../common-source/Utils/Log.h"

ClientsManager::ClientsManager(Server& server) :
    server(server)
{
    //ctor
}

ClientsManager::~ClientsManager()
{
    //dtor
}

bool ClientsManager::addClient(IpAndPort& client, const std::string &nickname, Player* player)
{
    if (clients.find(client) != clients.end())
        return false;

    clients.emplace(std::pair<IpAndPort, std::unique_ptr<Client>>(client, std::make_unique<Client>(server, client, nickname, player)));

    // first connected client
    if (clients.size() == 1)
    {
        clients[client]->admin = true;
    }

    return true;
}

void ClientsManager::removeClient(IpAndPort& client)
{
    auto i = clients.find(client);

    if (i == clients.cend())
        return;

    assert(!i->second->hasPlayer());

    clients.erase(i);
}

Client& ClientsManager::getClient(IpAndPort& client) const
{
    auto i = clients.find(client);

    if (i == clients.end())
        throw new std::logic_error("getClient: client doesn't exist.");

    return *i->second;
}

void ClientsManager::resetClientTimer(IpAndPort& client)
{
    if (clients.find(client) != clients.end())
        getClient(client).last_packet_received.restart();
}

int ClientsManager::doTimeOuts(float timeout_s)
{
    int yeet_count = 0;

    for (auto i = clients.begin(); i != clients.end(); )
    {
        if (i->second->last_packet_received.getElapsedTime().asSeconds() > timeout_s)
        {
            #ifdef SOLO
            if (i->first == server.owner)
            {
                server.running = false;
                break;
            }
            #endif // SOLO
            log(INFO, "Disconnecting client {}:{} (timeout).\n", i->first.address, i->first.port);

            Client& client = *(i->second);

            if (client.hasPlayer())
            {
                server.getWorld().getEntityManager().removeEntity(client.getPlayer()->getId());
                client.setPlayer(nullptr);
            }

            i = clients.erase(i);
            yeet_count++;

            log(INFO, "Client disconnected.\n");
        }
        else
            i++;
    }

    return yeet_count;
}

void ClientsManager::sendToAll(ECCPacket& packet)
{
    for (auto i = clients.begin(); i != clients.end(); i++)
        i->second->send(packet);
}
