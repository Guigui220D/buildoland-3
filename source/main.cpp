#include "Game.h"

#include <cstdlib>
#include <ctime>

int main()
{
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
