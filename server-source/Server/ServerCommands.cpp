#include "Server.h"

#include "../../common-source/Utils/Log.h"

void Server::handleCommand(const std::string &command)
{
    if (command == "test")
    {
        log(INFO, "Test command!!\n");
    }
}
