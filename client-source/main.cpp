#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Version.h"
#include "../common-source/Utils/Log.h"

int main()
{
    std::srand(std::time(nullptr));

    log_streams_by_level[INFO].push_back(&std::clog);
    log_streams_by_level[WARN].push_back(&std::cerr);
    log_streams_by_level[ERROR].push_back(&std::cerr);
    min_log_level = INFO;

    log(INFO, "Running Buildoland 3 version {} (short: {})\n",  Version::VERSION, Version::VERSION_SHORT);

    Game game;

    log(ERROR, "hello world {} !\n", 42.007);

    log(INFO, "Initializing game...\n");

    int init_code = game.init();
    if (init_code)
        return init_code;

    log(INFO, "Initialized!\n");
    log(INFO, "Starting game.\n");

    int run_exit_code = game.run();
    if (run_exit_code)
        return run_exit_code;

    log(INFO, "Exiting...\n");

    game.exit();
    return 0;
}
