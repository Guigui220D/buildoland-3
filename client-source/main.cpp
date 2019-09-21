#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Version.h"

int main()
{
    std::clog << "Running Buildoland 3 version " << Version::VERSION << " (short: " << Version::VERSION_SHORT << ")\n" << std::endl;

    std::srand(std::time(nullptr));

    Game game;

    std::clog << "Initializing game..." << std::endl;

    int init_code = game.init();
    if (init_code)
        return init_code;

    std::clog << "Initialized!" << std::endl;
    std::clog << "Starting game." << std::endl;

    int run_exit_code = game.run();
    if (run_exit_code)
        return run_exit_code;

    std::clog << "Exiting..." << std::endl;

    game.exit();
    return 0;
}
