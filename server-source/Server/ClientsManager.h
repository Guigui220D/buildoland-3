#pragma once

#include <map>
#include <memory>

#include "Client.h"

class Server;

class ClientsManager
{
    public:
        ClientsManager(Server& server);
        ~ClientsManager();

        /**
         * Checks if a client is connect
         * @param client : the ip and port of the client
         * @return True if the ip and port has a corresponding client object
         */
        inline bool isConnected(IpAndPort client) const { return clients.find(client) != clients.cend(); }

        /**
         * Creates a client object for the client and adds it to the map
         * The client isnt added if it exists already
         * @param client : the ip and port of the client
         * @param player : a pointer to a player to associate to this client (unlikely to be used)
         * @return True the client was added
         */
        bool addClient(IpAndPort& client, const std::string& nickname, Player* player = nullptr);
        /**
         * Removes a client
         * The client must NOT have a player. If it does, it should be removed before.
         * @param client : the ip and port of the client
         */
        void removeClient(IpAndPort& client);
        /**
         * Gets a reference to a client
         * The client must exist, check with isConnected()
         * @param client : the ip and port of the client
         * @return A reference to the client
         */
        Client& getClient(IpAndPort& client) const;

        /**
         * Resets the timeout timer of a client
         * Should be called when that client receives a valid packet
         * @param client : the ip and port of the client
         */
        void resetClientTimer(IpAndPort& client);
        /**
         * Removes all clients that have their timeout timer higher than the timeout value
         * @param timeout_s : the timeout value, in seconds
         * @return The number of clients that were disconnected
         */
        int doTimeOuts(float timeout_s);

        /**
         * Sends a packet to all clients
         * @param packet : the packet to send
         */
        void sendToAll(ECCPacket& packet);

        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsBegin() const { return clients.cbegin(); }
        inline std::map<IpAndPort, std::unique_ptr<Client>>::const_iterator
            getClientsEnd() const { return clients.cend(); }

    private:
        Server& server;

        std::map<IpAndPort, std::unique_ptr<Client>> clients;
};
