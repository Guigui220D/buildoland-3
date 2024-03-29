#pragma once

#include <atomic>

namespace sf
{
class Event;
class RenderTarget;
}

class Game;

class State
{
    public:
        //The constructor should not take time! every time consuming action should be done in a init function
        State(Game& game, unsigned int id = 0);
        virtual ~State();

        virtual void init();
        /**
         * Takes an event and handle it, or not.
         * @param event : the even to handle
         * @return False if the even should be transmitted to the next state.
         */
        virtual bool handleEvent(sf::Event& event);
        /**
         * Does a tick to update pretty much anything
         * @param delta_time : the seconds that have passed since the last update
         */
        virtual void update(float delta_time);
        /**
         * Draws the state's content on the render target
         * @param target : where the screen needs to be drawn
         */
        virtual void draw(sf::RenderTarget& target) const;
        /**
         * Updates view
         */
        virtual void updateView();

        /**
         * @return True if the states underneath should be updated
         */
        inline bool isUpdateTransparent() const { return update_transparent; }
        /**
         * @return True if the states underneath should be drawn
         */
        inline bool isDrawTransparent() const { return draw_transparent; }

        /**
         * To get the ID of the state. The id of a state is used if we need remove a state that is not at the top or bottom
         * @return The id of the state
         */
        inline unsigned int getID() const { return id; }
        /**
         * To get the game this state belongs to
         * @return The pointer to the game
         */
        inline Game& getGame() const { return game; }

        virtual inline bool isMenu() const { return false; }

        virtual void notifyStateStackChange();

        bool isTopState() const;

        std::atomic<bool> must_be_destroyed {false};

    protected:
        // If true, the next state should be updated/drawn
        bool update_transparent, draw_transparent;

    private:
        Game& game;
        unsigned int id;
};
