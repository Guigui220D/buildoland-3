#include "Client.h"

#include <assert.h>
#include <iostream>

#include "Server.h"

Client::Client(Server& server, IpAndPort ip_and_port, Player* player) :
    ip_and_port(ip_and_port),
    player(player),
    server(server)
{
    //ctor
}

Client::~Client()
{
    //dtor
}

void Client::send(ECCPacket& packet) const
{
    server.server_socket.send(packet, ip_and_port.address, ip_and_port.port);
}
