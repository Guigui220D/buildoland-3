#include "Game.h"

int main()
{
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
