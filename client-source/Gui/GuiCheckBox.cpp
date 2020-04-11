#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(Game& game) :
    GuiButton(game, sf::Vector2f(), 100.f, ""),
    checked(false)
{
    //ctor
}

GuiCheckBox::~GuiCheckBox()
{
    //dtor
}

void GuiCheckBox::update(float delta_time)
{
    if (hasBeenClicked())
        checked = !checked;

    if (checked)
        button.setFillColor(isMouseInside() ? sf::Color(127, 255, 0) : sf::Color::Green);
    else
        button.setFillColor(isMouseInside() ? sf::Color(255, 127, 0) : sf::Color::Red);
}
