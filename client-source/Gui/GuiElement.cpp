#include "GuiElement.h"

#include "../Game.h"

GuiElement::GuiElement(Game& game) :
    game(game)
{
}

GuiElement::~GuiElement()
{
    //dtor
}

void GuiElement::init() {}

bool GuiElement::handleEvent(sf::Event& event)
{
    return false;
}

void GuiElement::draw(sf::RenderTarget& target) const {}

void GuiElement::update(float delta_time) {}

void GuiElement::setTransparency(uint8_t fade) {}
