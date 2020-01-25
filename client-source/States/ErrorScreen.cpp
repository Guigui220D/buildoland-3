#include "ErrorScreen.h"

#include "../Game.h"
#include "LoadingScreenState.h"
#include "GameState.h"

ErrorState::ErrorState(Game& game, sf::String error, unsigned int id) :
    State(game, id),
    go_back_button(game, sf::FloatRect(.1f, .7f, .8f, .1f), sf::Vector2f(8.f, 1.f), GuiAlign::Center, GuiAlign::Center, "Back to main menu"),
    message(game, sf::FloatRect(.05f, .4f, .9f, .2f), sf::Vector2f(8.f, 1.f), GuiAlign::Center, GuiAlign::Center, error)
{
    draw_transparent = false;
}

ErrorState::~ErrorState()
{
    //dtor
}

void ErrorState::init()
{
    go_back_button.init();
    message.init();
}

bool ErrorState::handleEvent(sf::Event& event)
{
    if (go_back_button.handleEvent(event))
        return true;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    case sf::Event::Resized:
        go_back_button.calculateView(sf::Vector2u(event.size.width, event.size.height));
        message.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void ErrorState::update(float delta_time)
{
    go_back_button.update(delta_time);

    if (go_back_button.hasBeenClicked())
        must_be_destroyed = true;
}

void ErrorState::draw(sf::RenderTarget& target) const
{
    go_back_button.draw(target);
    message.draw(target);
}

void ErrorState::updateView()
{
    go_back_button.calculateView(getGame().getWindow().getSize());
    message.calculateView(getGame().getWindow().getSize());
}
