#include "Game.h"

#include <iostream>
#include <assert.h>

#include "States/LoadingScreenState.h"
#include "States/States.h"

Game::Game()
{
    settings_manager.load();
    auto& ws = settings_manager.window_settings;
    //auto& as = settings_manager.window_settings;

    window.create(sf::VideoMode(ws.size_x, ws.size_y), "BuildOLand 3", ws.type);
    window.setFramerateLimit(ws.fps_limit);
    window.setVerticalSyncEnabled(ws.vsync_enabled);

    gameBlocksManager.initBlocks();
    gameGroundsManager.initGrounds();
}

Game::~Game()
{
    //dtor
}

int Game::init()
{
    addStateOnTop(new BackgroundState(this), true);
    //Test
        addStateOnTop(new LoadingScreenState<MainMenuState>(false, true, this, 0));
    return 0;
}

int Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {

        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                std::cout << states_stack.size() << std::endl;
                break;
            default:
                {
                    for (int i = states_stack.size() - 1; i >= 0; i--)
                    {
                        std::unique_ptr<State>& state = states_stack.at(i);
                        bool caught = state->handleEvent(e);
                        if (caught)
                            break;
                    }
                }
                break;
            }
        }

        update(clock.restart().asSeconds());

        window.clear();
        draw();
        window.display();

        //Remove states
        for (auto i = states_stack.begin(); i != states_stack.end(); )
        {
            if ((*i)->must_be_destroyed)
            {
                (*i).reset();
                states_stack.erase(i);
            }
            else i++;
        }
        //Add states on top
        for (State*& state : states_to_add_on_top)
            states_stack.push_back(std::unique_ptr<State>(state));
        states_to_add_on_top.clear();
        //Add states under the top
        if (state_to_add_under_the_top)
        {
            assert(states_stack.size());
            states_stack.insert(states_stack.end() - 1, std::unique_ptr<State>(state_to_add_under_the_top));
            state_to_add_under_the_top = nullptr;
        }
    }
    return 0;
}

void Game::exit()
{

}


void Game::addStateOnTop(State* state, bool init)
{
    if (init)
        state->init();
    states_to_add_on_top.push_back(state);
}

void Game::addStateUnderTop(State* state, bool init)
{
    assert(!state_to_add_under_the_top);
    if (init)
        state->init();
    state_to_add_under_the_top = state;
}


void Game::draw()
{
    if (states_stack.empty())
        return;

    unsigned int i;
    for (i = states_stack.size() - 1; i > 0; i--)
    {
        if (!states_stack.at(i)->isDrawTransparent())
            break;
    }
    for (; i < states_stack.size(); i++)
        states_stack.at(i)->draw(window);
}

void Game::update(float delta_time)
{
    for (int i = states_stack.size() - 1; i >= 0; i--)
    {
        std::unique_ptr<State>& state = states_stack.at(i);
        state->update(delta_time);
        if (!state->isUpdateTransparent())
            break;
    }
}
