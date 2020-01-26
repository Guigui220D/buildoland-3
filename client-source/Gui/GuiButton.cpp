#include "GuiButton.h"

#include "../Game.h"

GuiButton::GuiButton(Game& game, sf::FloatRect zone, sf::Vector2f size, GuiAlign horizontal_align, GuiAlign vertical_align, sf::String text_string) :
    GuiElement(game, zone, size, horizontal_align, vertical_align),
    string(text_string)
{
    button.setSize(size);
    button.setFillColor(sf::Color::White);
}

GuiButton::~GuiButton()
{
    //dtor
}

void GuiButton::init()
{
    text.setString(game.getLanguageManager().getString(string));
    text.setCharacterSize(100);
    text.scale(sf::Vector2f(1.f / 120.f, 1.f / 120.f));
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setFillColor(sf::Color::Black);

    sf::Vector2f text_size = sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height);
    sf::Vector2f correction(0.f, -.3f);

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
    useView(target);
    target.draw(button);
    target.draw(text);
}

void GuiButton::update(float delta_time)
{
    button.setFillColor(isMouseInside() ? sf::Color::Yellow : sf::Color::White);
}

bool GuiButton::isMouseInside() const
{
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(getGame().getWindow());
    sf::Vector2f coords = getGame().getWindow().mapPixelToCoords(mouse_pos, getView());
    return (coords.x >= 0.f && coords.y >= 0.f && coords.x <= getAspectRatio() && coords.y <= 1.f);
}
