#include <iostream>

#include "Server.h"
#include "Version.h"

int main()
{
    #ifdef SOLO
        std::cout << "This is a local server for solo mode." << std::endl;
        std::cout << "This version is made for Buildoland " << Version::VERSION << " (" << Version::VERSION_SHORT << ")\n" << std::endl;
    #else
        std::cout << "This is a multiplayer server." << std::endl;
        std::cout << "This version is made for Buildoland " << Version::VERSION << " (" << Version::VERSION_SHORT << ")\n" << std::endl;
    #endif // SOLO


    Server server;

    if (!server.init(58888))
    {
        std::cerr << "Fatal : Could not start server." << std::endl;
        return 1;
    }
    std::cout << "\nServer started!\n" << std::endl;

    server.run();

    return 0;
}
