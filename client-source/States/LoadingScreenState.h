#pragma once

#include "../Gui/GuiLoadingIcon.h"
#include "State.h"

template <class T>
class LoadingScreenState : public State
{
    public:
        static_assert(std::is_base_of<State, T>::value, "The loading screen must load a state.");

        template <typename... Args>
        LoadingScreenState(bool fade_in, bool fade_out, Game& game, unsigned int id, Args&&... args);
        ~LoadingScreenState();

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;

    protected:
        /**
         * The task to be done before initializing the next state
         * Should be overridden
         */
        virtual void beforeInitTask();
        /**
         * The task to be done after initializing the next state
         * Should be overridden
         */
        virtual void afterInitTask();

        State* state_being_loaded;
        /**
         * Loads the state to be loaded, this function is supposed to be async
         */
        void load();
        sf::Thread loading_thread;

        std::atomic<bool> done = false;
        bool working = false;
        sf::Clock working_time;

        sf::Clock fade_clock;
        int fade = 0;
        bool fade_in, fade_out;

        sf::View view;
        sf::RectangleShape background;
        GuiLoadingIcon loading_icon;

        inline uint8_t getFade() { return fade; };

    private:
        /**
         * To draw more on child classes without breaking this class's draw
         * @param target : the render target to draw on
         */
        virtual void drawMore(sf::RenderTarget& target) const;
        /**
         * To update more on child classes without breaking this class's update function
         * @param delta_time : the time elapsed
         */
        virtual void updateMore(float delta_time);
};

#include "LoadingScreenState_Impl.h"
