#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(Game& game, sf::FloatRect zone, GuiAlign horizontal_align, GuiAlign vertical_align) :
    GuiButton(game, zone, 1.f, horizontal_align, vertical_align, ""),
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
