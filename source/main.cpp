#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Version.h"

int main()
{
    std::cout << "Running Buildoland 3 version " << Version::VERSION << " (short: " << Version::VERSION_SHORT << ")\n" << std::endl;

    std::srand(std::time(nullptr));

    Game game;

    int init_code = game.init();
    if (init_code)
        return init_code;

    int run_exit_code = game.run();
    if (run_exit_code)
        return run_exit_code;

    game.exit();
    return 0;
}
