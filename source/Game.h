#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "States/State.h"

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

        /**
         * Adds a state to the top of the states stack using its pointer
         * The pointer will be then managed and deleted automatically
         * @param state : the pointer to the state
         */
        void addStateOnTop(State* state, bool init = false);
        /**
         * Adds a state to the states stack using its pointer just under the top
         * The pointer will be then managed and deleted automatically
         * Used by LoadingScreenState
         * @param state : the pointer to the state
         */
        void addStateUnderTop(State* state, bool init = false);

        /**
         * Gets a reference to the window this game owns
         * @return A reference to the render window
         */
        inline sf::RenderWindow& getWindow() { return window; }

    private:
        sf::RenderWindow window;

        unsigned int framerate_target = 60;

        //All the states currently loaded
        std::vector<std::unique_ptr<State>> states_stack;
        std::vector<State*> states_to_add;

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
