#include <iostream>

#include "Server.h"
#include "Version.h"

int main(int argc, char** argv)
{
    uint16_t client_port = 0;

    #ifdef SOLO
        std::cout << "This is a local server for solo mode." << std::endl;
        std::cout << "This version is made for Buildoland " << Version::VERSION << " (" << Version::VERSION_SHORT << ")\n" << std::endl;

        if (argc < 2)
        {
            std::cerr << "Fatal : Expected client's port for handshake in solo mode (as an argument)." << std::endl;
            return 1;
        }

        {
            int port_int = std::atoi(argv[1]);
            if (port_int <= 0 || port_int >= 65536)
            {
                std::cerr << "Fatal : Wrong port was given : " << port_int << " is not a valid port." << std::endl;
                return 1;
            }
            client_port = port_int;
        }
        std::cout << "Client has port " << client_port << std::endl;

    #else
        std::cout << "This is a multiplayer server." << std::endl;
        std::cout << "This version is made for Buildoland " << Version::VERSION << " (" << Version::VERSION_SHORT << ")\n" << std::endl;


    #endif // SOLO

    Server server(client_port);

    if (!server.init(58888))
    {
        std::cerr << "Fatal : Could not start server." << std::endl;
        return 1;
    }
    std::cout << "\nServer started!\n" << std::endl;

    server.run();

    std::cout << "Server ended." << std::endl;

    return 0;
}