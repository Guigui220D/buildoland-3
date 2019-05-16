#include "BackgroundState.h"

#include <iostream>

#include "../Game.h"

BackgroundState::BackgroundState(Game* game, unsigned int id) :
    State(game, id)
{
    update_transparent = false;
    draw_transparent = false;

    view = game->getWindow().getView();
    //Temporary!! Texture manager
        background_texture.loadFromFile("Res/Temp/background.png");
        background_rectangle.setTexture(&background_texture);
        background_rectangle.setSize(sf::Vector2f(800.f, 600.f));
}

BackgroundState::~BackgroundState()
{
    //dtor
}

bool BackgroundState::handleEvent(sf::Event& event)
{
    return true;
}

void BackgroundState::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    target.draw(background_rectangle);
}
