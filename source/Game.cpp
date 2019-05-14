#include "Game.h"

Game::Game() :
    window(sf::VideoMode(800, 600), "SFML window")
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
    return 0;
}

int Game::run()
{
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
    return 0;
}

void Game::exit()
{

}
