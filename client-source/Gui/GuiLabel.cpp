#include "GuiLabel.h"

GuiLabel::~GuiLabel()
{

}

void GuiLabel::init()
{
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setCharacterSize(100);
    text.scale(sf::Vector2f(8.f / text.getGlobalBounds().width, 8.f / text.getGlobalBounds().width));
}

void GuiLabel::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(text);
}
