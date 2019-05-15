#include "Game.h"

#include "States/MainMenuState.h"

Game::Game() :
    window(sf::VideoMode(800, 600), "What's the name?")
{
    //ctor
}

Game::~Game()
{
    //dtor
}

int Game::init()
{
    window.setFramerateLimit(framerate_target);
    //Test
        states_stack.push_back(std::make_unique<MainMenuState>());
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
            }
        }

        update(clock.restart().asSeconds());

        window.clear();
        draw();
        window.display();
    }
    return 0;
}

void Game::exit()
{

}



void Game::draw()
{
    for (int i = states_stack.size() - 1; i >= 0; i--)
    {
        std::unique_ptr<State>& state = states_stack.at(i);
        state->draw(window);
        if (!state->isDrawTransparent())
            break;
    }
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
