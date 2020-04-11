#include "GuiButton.h"

#include "../Game.h"

GuiButton::GuiButton(Game& game, sf::Vector2f position, float width, sf::String text_string) :
    GuiElement(game),
    string(text_string)
{
    button.setSize(sf::Vector2f(width, 100.f));
    button.setPosition(position);
}

GuiButton::~GuiButton()
{
    //dtor
}

void GuiButton::init()
{
    button.setFillColor(sf::Color::White);

    text.setString(game.getLanguageManager().getString(string));
    text.setCharacterSize(200);
    text.scale(sf::Vector2f(.4f, .4f));
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setFillColor(sf::Color::Black);

    sf::Vector2f text_size = sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height);
    sf::Vector2f correction(0.f, -30.f);

    text.setPosition(button.getPosition() + button.getSize() * .5f - text_size * .5f + correction);
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
    target.draw(button);
    target.draw(text);
}

void GuiButton::update(float delta_time)
{
    button.setFillColor(isMouseInside() ? sf::Color::Yellow : sf::Color::White);
}

bool GuiButton::isMouseInside() const
{
    sf::Vector2f coords = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));

    sf::Vector2f pos = button.getPosition();
    sf::Vector2f corner = pos + button.getSize();

    return (coords.x >= pos.x && coords.y >= pos.y && coords.x <= corner.x && coords.y <= corner.y);
}
