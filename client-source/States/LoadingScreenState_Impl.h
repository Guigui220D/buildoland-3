#include "../Game.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../common-source/Utils/Log.h"

template <class T>
template <typename... Args>
LoadingScreenState<T>::LoadingScreenState(bool fade_in, bool fade_out, Game& game, unsigned int id, Args&&... args) :
    State(game, id),
    done(false),
    fade_in(fade_in),
    fade_out(fade_out),
    view(sf::Vector2f(.5f, .5f), sf::Vector2f(1.f, 1.f)),
    loading_icon(game)
{
    update_transparent = false;
    draw_transparent = true;

    background.setFillColor(sf::Color::Black);
    background.setSize(sf::Vector2f(1.f, 1.f));

    state_being_loaded = new T(game, id, args...);
}

template <class T>
LoadingScreenState<T>::~LoadingScreenState()
{
    //dtor
}

template <class T>
void LoadingScreenState<T>::load()
{
    sf::Clock clk;
    beforeInitTask();
    state_being_loaded->init();
    afterInitTask();
    state_being_loaded->updateView();
    //while (clk.getElapsedTime().asSeconds() < 2.f); //Lol, making fake loading time
    done = true;
    //log(INFO,"Loading took {} seconds (still in thread).\n", clk.getElapsedTime().asSeconds());
    return;
}

template <class T>
void LoadingScreenState<T>::beforeInitTask() {}

template <class T>
void LoadingScreenState<T>::afterInitTask() {}

template <class T>
bool LoadingScreenState<T>::handleEvent(sf::Event& event)
{
    return true;
}

template <class T>
void LoadingScreenState<T>::update(float delta_time)
{
    if (done)
    {
        if (working)
        {
            log(INFO,"Loading took {} seconds.\n", working_time.getElapsedTime().asSeconds());
            working = false;
            fade_clock.restart();
            getGame().addStateUnderTop(state_being_loaded, false);
        }

        if (fade_clock.getElapsedTime().asMilliseconds())
            fade -= fade_out ? fade_clock.restart().asMilliseconds() : 255;
        if (fade <= 0)
        {
            if (loading_thread.joinable())
                loading_thread.join();
            must_be_destroyed = true;
            fade = 0;
        }
    }
    else
    {
        if (fade_clock.getElapsedTime().asMilliseconds())
            fade += fade_in ? fade_clock.restart().asMilliseconds() : 255;
        if (fade >= 255)
        {
            fade = 255;
            if (!working)
            {
                working = true;
                working_time.restart();
                loading_thread = std::thread(&LoadingScreenState::load, this);
            }
        }
    }

    background.setFillColor(sf::Color(0, 0, 0, fade));
    if (working)
        loading_icon.update(delta_time);
    updateMore(delta_time);
}

template <class T>
void LoadingScreenState<T>::updateMore(float delta_time) {}

template <class T>
void LoadingScreenState<T>::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    target.draw(background);
    //if(working && working_time.getElapsedTime().asSeconds() > .3f)
    //    loading_icon.draw(target);
    drawMore(target);
}

template <class T>
void LoadingScreenState<T>::drawMore(sf::RenderTarget& target) const {}

