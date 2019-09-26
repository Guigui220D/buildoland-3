#include <iostream>

#include "Server/Server.h"
#include "Version.h"

#include <ctime>
#include <cstdlib>

//To hide console
#include <windows.h>

int main(int argc, char** argv)
{
    bool hide = false;

    std::srand(std::time(0));

    uint16_t client_port = 0;
    uint16_t server_port = 0;

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

        if (argc >= 3/* && strcmp(argv[2], "hide")*/)
        {
            hide = true;
            std::cout << "Going to hide console." << std::endl;
        }

    #else
        std::cout << "This is a multiplayer server." << std::endl;
        std::cout << "This version is made for Buildoland " << Version::VERSION << " (" << Version::VERSION_SHORT << ")\n" << std::endl;


    #endif // SOLO

    Server server(client_port);

    //In local/solo mode, the port is 0 which means the os gets to choose the port
    //In multiplayer mode, we prefer chosing a port
    #ifndef SOLO
    server_port = 58888;
    #endif // SOLO

    if (!server.init(server_port))
    {
        std::cerr << "Fatal : Could not start server." << std::endl;
        return 1;
    }

    if (hide)
    {
        //TODO : Other platforms
        HWND hWnd = GetConsoleWindow();
        ShowWindow(hWnd, SW_MINIMIZE);
        ShowWindow(hWnd, SW_HIDE);
    }

    std::cout << "\nServer started!\n" << std::endl;

    server.run();

    std::cout << "Server ended." << std::endl;

    //while (1) {}

    return 0;
}
