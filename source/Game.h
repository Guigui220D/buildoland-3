#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "State.h"

class Game
{
    public:
        Game();
        ~Game();

        /**
         * Initializes everything the game needs (assets, managers, etc...)
         * @return A code, 0 if success
         */
        int init();
        /**
         * Runs the game loop. Please run init() before and exit() after.
         * @return A code, 0 end of the loop reached
         */
        int run();
        /**
         * Unloads and un-allocate what needs to be
         */
        void exit();

    private:
        sf::RenderWindow window;

        unsigned int framerate_target = 60;

        //All the states currently loaded
        std::vector<std::unique_ptr<State>> states_stack;

        /**
         * Draws states that need to be on the window
         */
        void draw();
        /**
         * Updates states that need to be
         * @param delta_time : the seconds that have passed since the last update
         */
        void update(float delta_time);
};
