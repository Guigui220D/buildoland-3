#pragma once

#include <SFML/Graphics.hpp>

class Game
{
    public:
        Game();
        ~Game();

        int init();
        int run();
        void exit();

    private:
        sf::RenderWindow window;

        unsigned int framerate_target = 60;
};
