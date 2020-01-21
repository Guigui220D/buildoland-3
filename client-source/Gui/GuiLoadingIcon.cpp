#include "GuiLoadingIcon.h"

GuiLoadingIcon::GuiLoadingIcon(Game& game) :
    GuiElement(game, sf::FloatRect(.87f, .87f, .1f, .1f), 1.f, GuiAlign::BottomOrRight, GuiAlign::BottomOrRight)
{
    icon.setSize(sf::Vector2f(1.f, 1.f));
    icon.setFillColor(sf::Color::White);
    icon.setOrigin(sf::Vector2f(.5f, .5f));
    icon.setPosition(sf::Vector2f(.5f, .5f));
}

GuiLoadingIcon::~GuiLoadingIcon()
{
    //dtor
}

void GuiLoadingIcon::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(icon);
}

void GuiLoadingIcon::update(float delta_time)
{
    icon.rotate(delta_time * 100.f);
}
