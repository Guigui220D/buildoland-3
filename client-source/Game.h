#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "States/State.h"

#include "Settings/SettingsManager.h"
#include "Res/ResourceManager.h"
#include "Res/AudioManager.h"

#include "Block/GameBlocks.h"
#include "Ground/GameGrounds.h"

class Game
{
    public:
        Game();
        ~Game();

        /**
         * Should only be used for adding the first states
         * Resources are loaded async by one of the states
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
        void addStateOnTop(State* state, bool init = true);
        /**
         * Adds a state to the states stack using its pointer just under the top
         * The pointer will be then managed and deleted automatically
         * Used by LoadingScreenState
         * @param state : the pointer to the state
         */
        void addStateUnderTop(State* state, bool init = true);

        /**
         * Gets a reference to the window this game owns
         * @return A reference to the render window
         */
        inline sf::RenderWindow& getWindow() { return window; }
        /**
         * Gets a reference to the blocks manager
         * @return A reference to the blocks manager
         */
        inline const GameBlocks& getBlocksManager() const { return game_blocks_manager; }
        /**
         * Gets a reference to the grounds manager
         * @return A reference to the grounds manager
         */
        inline const GameGrounds& getGroundsManager() const { return game_grounds_manager; }
        /**
         * Gets a reference to the resource manager
         * @return A reference to the resource manager
         */
        inline const ResourceManager& getResourceManager() const { return resource_manager; }
        /**
         * Gets a reference to the audio manager
         * @return A reference to the audio manager
         */
        inline AudioManager& getAudioManager() { return audio_manager; }

    private:
        friend class TitleScreenState;

        sf::RenderWindow window;

        SettingsManager settings_manager;

        GameBlocks game_blocks_manager;
        GameGrounds game_grounds_manager;

        ResourceManager resource_manager;
        AudioManager audio_manager;

        void loadResources();

        unsigned int framerate_target = 60;

        //All the states currently loaded
        std::vector<std::unique_ptr<State>> states_stack;
        std::vector<State*> states_to_add_on_top;
        State* state_to_add_under_the_top = nullptr; //Wow long name

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
