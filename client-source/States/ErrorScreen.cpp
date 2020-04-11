#include "ErrorScreen.h"

#include "../Game.h"
#include "GameState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

ErrorState::~ErrorState()
{
    //dtor
}

void ErrorState::init()
{
    gui_zone.addElement(go_back_button);
    gui_zone.addElement(message);
    gui_zone.init();
}

bool ErrorState::handleEvent(sf::Event& event)
{
    if (gui_zone.handleEvent(event))
        return true;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    case sf::Event::Resized:
        gui_zone.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void ErrorState::update(float delta_time)
{
    gui_zone.update(delta_time);

    if (go_back_button->hasBeenClicked())
        must_be_destroyed = true;
}

void ErrorState::draw(sf::RenderTarget& target) const
{
    //gui_zone.debugDraw(target);
    gui_zone.draw(target);
}

void ErrorState::updateView()
{
    gui_zone.calculateView(getGame().getWindow().getSize());
}
