#include <iostream>

#include "Server/Server.h"
#include "Version.h"

#include <ctime>
#include <cstdlib>

#ifdef __unix__
    #include "signal.h"
#endif // __unix__

#include "../common-source/Utils/Log.h"

#include "../external/stacktrace/stacktrace.hpp"

Server* serv = nullptr;

void sigint(int dummy)
{
    if (serv)
        serv->stop();

    signal(SIGINT, SIG_DFL);
}

int main(int argc, char** argv)
{
    #ifdef __unix__
    signal(SIGINT, sigint);
    #endif // __unix__

    std::srand(std::time(0));

    log_streams_by_level[INFO].push_back(&std::clog);
    log_streams_by_level[WARN].push_back(&std::cerr);
    log_streams_by_level[ERROR].push_back(&std::cerr);
    min_log_level = INFO;

    init_stack_trace_handler(argc, argv);

    uint16_t client_port = 0;
    uint16_t server_port = 0;

#ifdef SOLO
    log_prefix_format = "[Server] "; // to differentiate output when using solo mode

    log(INFO, "This is a local server for solo mode.\n");
    log(INFO, "This version is made for Buildoland {} ({})\n", Version::VERSION, Version::VERSION_SHORT);

    if (argc < 2)
    {
        log(ERROR, "Fatal : Expected client's port for handshake in solo mode (as an argument).\n");
        return 1;
    }

    {
        int port_int = std::atoi(argv[1]);
        if (port_int <= 0 || port_int >= 65536)
        {
            log(ERROR, "Fatal : Wrong port was given : {} is not a valid port.\n", port_int);
            return 1;
        }
        client_port = port_int;
    }
    log(INFO, "Client has port {}\n", client_port);

#else
    log(INFO, "This is a multiplayer server.\n");
    log(INFO, "This version is made for Buildoland {} ({})\n", Version::VERSION, Version::VERSION_SHORT);


#endif // SOLO

    {   //Scope to for
        Server server(client_port);
        serv = &server;

//In local/solo mode, the port is 0 which means the os gets to choose the port
//In multiplayer mode, we prefer chosing a port
#ifndef SOLO
        server_port = 58888;
#endif // SOLO

        if (!server.init(server_port))
        {
            log(ERROR, "Fatal : Could not start server.\n");
            return 1;
        }

        log(INFO, "Server started!\n\n");

        server.run();

        log(INFO, "Ending server\n");

        server.close();

        serv = nullptr;
    }

    log(INFO, "Server ended.\n");

    //while (1) {}

    return 0;
}
