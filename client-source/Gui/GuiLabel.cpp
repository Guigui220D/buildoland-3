#include "GuiLabel.h"

#include "../Game.h"

GuiLabel::GuiLabel(Game* game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align, const std::string message) :
    GuiElement(game, zone, aspect_ratio, horizontal_align, vertical_align),
    message(message)
{

}

GuiLabel::~GuiLabel()
{

}

void GuiLabel::init()
{
    text.setString(message);
    text.setFont(getGame()->getResourceManager().getFont("GUI_FONT"));
    text.setCharacterSize(100);
    text.scale(sf::Vector2f(8.f / text.getGlobalBounds().width, 8.f / text.getGlobalBounds().width));
}

void GuiLabel::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(text);
}
