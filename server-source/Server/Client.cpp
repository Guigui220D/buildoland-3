#include "Client.h"

#include "Server.h"

Client::Client(Server& server, IpAndPort ip_and_port, const std::string &in_nickname, Player* player) :
    ip_and_port(ip_and_port),
      nickname(in_nickname),
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
