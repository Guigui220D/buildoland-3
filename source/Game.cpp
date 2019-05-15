#include "Game.h"

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
        states_stack.push_back(std::make_unique<State>());
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
            if (e.type == sf::Event::Closed)
                window.close();
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
    for (std::unique_ptr<State>& state : states_stack)
    {
        state->draw(window);
        if (!state->isDrawTransparent())
            break;
    }
}

void Game::update(float delta_time)
{
    for (std::unique_ptr<State>& state : states_stack)
    {
        state->update(delta_time);
        if (!state->isUpdateTransparent())
            break;
    }
}
