#include "GuiButton.h"

#include "../Game.h"

GuiButton::GuiButton(Game* game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align) :
    GuiElement(game, zone, aspect_ratio, horizontal_align, vertical_align)
{
    button.setSize(sf::Vector2f(aspect_ratio, 1));
    button.setFillColor(sf::Color::White);
}

GuiButton::~GuiButton()
{
    //dtor
}

bool GuiButton::handleEvent(sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        waiting_for_release = isMouseInside();
        if (waiting_for_release)
            return true;
        return false;
    case sf::Event::MouseButtonReleased:
        if (waiting_for_release)
        {
            if (isMouseInside())
            {
                clicked = true;
                return true;
            }
            waiting_for_release = false;
        }
        return false;
    default:
        return false;
    }
}

void GuiButton::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(button);
}

/*
void GuiButton::update(float delta_time)
{}
*/

bool GuiButton::isMouseInside() const
{
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(getGame()->getWindow());
    sf::Vector2f coords = getGame()->getWindow().mapPixelToCoords(mouse_pos, getView());
    return (coords.x >= 0.f && coords.y >= 0.f && coords.x <= getAspectRatio() && coords.y <= 1.f);
}
