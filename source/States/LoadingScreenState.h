#pragma once

#include <atomic>

#include "../Gui/GuiLoadingIcon.h"
#include "State.h"

template <class T>
class LoadingScreenState : public State
{
    public:
        static_assert(std::is_base_of<State, T>::value, "The loading screen must load a state.");

        template <typename... Args>
        LoadingScreenState(bool fade_in, bool fade_out, Game* game, unsigned int id, Args&&... args);
        virtual ~LoadingScreenState();

        virtual bool handleEvent(sf::Event& event) override;
        virtual void update(float delta_time) override;
        virtual void draw(sf::RenderTarget& target) const override;

    protected:
        State* state_being_loaded;
        /**
         * Loads the state to be loaded, this function is supposed to be async
         */
        virtual void load();
        sf::Thread loading_thread;

        sf::Mutex done_mutex;
        bool done = false;
        bool working = false;

        sf::Clock fade_clock;
        int fade = 0;
        bool fade_in, fade_out;

        sf::View view;
        sf::RectangleShape background;
        GuiLoadingIcon loading_icon;

    private:
};

//Because the way templates work i can't use a normal cpp file???
#include "LoadingScreenState_Impl.h"

