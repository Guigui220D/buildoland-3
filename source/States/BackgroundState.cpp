#include "BackgroundState.h"

#include <iostream>

#include "States.h"
#include "LoadingScreenState.h"
#include "../Game.h"

BackgroundState::BackgroundState(Game* game, unsigned int id) :
    State(game, id)
{
    update_transparent = false;
    draw_transparent = false;

    view = game->getWindow().getView();
    view.setSize(sf::Vector2f(800.f, 600.f));
    view.setCenter(view.getSize() / 2.f);
}

BackgroundState::~BackgroundState()
{
    //dtor
}

void BackgroundState::init()
{
    //Temporary!! Texture manager
        background_texture.loadFromFile("Res/Temp/background.png");
        background_rectangle.setTexture(&background_texture);
        background_rectangle.setSize(sf::Vector2f(800.f, 600.f));
}

bool BackgroundState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
        getGame()->addStateOnTop(new LoadingScreenState<MainMenuState>(true, true, getGame(), 0));
    return true;
}

void BackgroundState::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    target.draw(background_rectangle);
}
