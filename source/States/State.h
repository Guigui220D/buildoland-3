#pragma once

#include <SFML/Graphics.hpp>

class State
{
    public:
        State(unsigned int id = 0);
        virtual ~State();

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

    protected:
        // If true, the next state should be updated/drawn
        bool update_transparent, draw_transparent;

    private:
        unsigned int id;
};
