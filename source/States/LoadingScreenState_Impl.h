#include "../Game.h"

#include <iostream>

template <class T>
template <typename... Args>
LoadingScreenState<T>::LoadingScreenState(bool fade_in, bool fade_out, Game* game, unsigned int id, Args&&... args) :
    State(game, id),
    loading_thread(load, this),
    fade_in(fade_in),
    fade_out(fade_out),
    view(sf::Vector2f(.5f, .5f), sf::Vector2f(1.f, 1.f)),
    loading_icon(game)
{
    update_transparent = false;

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
    state_being_loaded->init();
    while (clk.getElapsedTime().asSeconds() < .2f);
    done_mutex.lock();
    done = true;
    done_mutex.unlock();
    return;
}


template <class T>
bool LoadingScreenState<T>::handleEvent(sf::Event& event)
{
    return true;
}

template <class T>
void LoadingScreenState<T>::update(float delta_time)
{
    done_mutex.lock();
    if (done)
    {
        if (working)
        {
            std::cout << "Loading took " << working_time.getElapsedTime().asSeconds() << " seconds." << std::endl;
            working = false;
            fade_clock.restart();
            getGame()->addStateUnderTop(state_being_loaded);
        }
        fade -= fade_out ? fade_clock.restart().asMilliseconds() : 255;
        if (fade <= 0)
        {
            must_be_destroyed = true;
            fade = 0;
        }
    }
    else
    {
        fade += fade_in ? fade_clock.restart().asMilliseconds() : 255;
        if (fade >= 255)
        {
            fade = 255;
            if (!working)
            {
                working = true;
                working_time.restart();
                loading_thread.launch();
            }
        }
    }
    done_mutex.unlock();
    background.setFillColor(sf::Color(0, 0, 0, fade));
    if (working)
        loading_icon.update(delta_time);
}

template <class T>
void LoadingScreenState<T>::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    target.draw(background);
    if(working && working_time.getElapsedTime().asSeconds() > .3f)
        loading_icon.draw(target);
    //loading_icon.debugDraw(target);
}

