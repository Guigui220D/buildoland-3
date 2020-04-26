#include "Server.h"

#include "../../common-source/Utils/Log.h"

void Server::initCommands()
{
    commands["test"] =
        {
            [](Client& client, const std::string& args)
            {
                log(INFO, "Test command, arguments : {}\n", args);
            },
            false // not admin
        };
    commands["whoami"] =
        {
            [](Client& client, const std::string& args)
            {
                sf::String msg;
                if (client.admin)
                    msg = "You are " + client.getNickname() + " ~ Administrator";
                else
                    msg = "You are " + client.getNickname();

                ECCPacket msg_packet(Networking::StoC::ReceivedMessage);
                msg_packet << ""; // system message
                msg_packet << msg;
                client.send(msg_packet);
            },
            false // not admin
        };


    // -------------  admin commands

    commands["stop"] =
        {
            [this](Client& client, const std::string& args)
            {
                running = false;
            },
            true // not admin
        };
}

void Server::handleCommand(Client& client, const std::string &command, const std::string& args)
{
    auto it = commands.find(command);
    if (it != commands.end())
    {
        if (it->second.admin_command && !client.admin)
        {
            sf::String msg = "Command refused, you are not an administrator.";
            ECCPacket msg_packet(Networking::StoC::ReceivedMessage);
            msg_packet << ""; // system message
            msg_packet << msg;
            client.send(msg_packet);
        }
        else
        {
            it->second.callback(client, args);
        }
    }
    else
    {
        sf::String msg = "Unknown command '" + command + "'";
        ECCPacket msg_packet(Networking::StoC::ReceivedMessage);
        msg_packet << ""; // system message
        msg_packet << msg;
        client.send(msg_packet);
    }
}
