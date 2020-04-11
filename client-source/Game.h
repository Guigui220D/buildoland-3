#pragma once

#include <memory>

namespace sf
{
class RenderWindow;
}

class GameBlocks;
class GameGrounds;
class ItemsRegister;
class ResourceManager;
class AudioManager;
class SettingsManager;
class LanguageManager;
class BindingsManager;

class State;

struct GameImpl;

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

        sf::RenderWindow &getWindow();

        const GameBlocks& getBlocksManager() const;
        const GameGrounds& getGroundsManager() const;

        const ItemsRegister& getItemsRegister() const;

        const ResourceManager &getResourceManager() const;
        AudioManager& getAudioManager();

        SettingsManager& getSettingsManager();
        LanguageManager& getLanguageManager();
        BindingsManager& getBindingsManager();

        void useDefaultView();

        State* getTopState() const;


    private:
        friend class TitleScreenState;

        /**
         * Draws states that need to be on the window
         */
        void draw();
        /**
         * Updates states that need to be
         * @param delta_time : the seconds that have passed since the last update
         */
        void update(float delta_time);

        void loadResources();
        void titleScreenInit();

        std::unique_ptr<GameImpl> pimpl;
};
