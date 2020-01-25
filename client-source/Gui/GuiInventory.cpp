#include "GuiInventory.h"

#include "../Game.h"

GuiInventory::GuiInventory(Game& game, PlayerInventory& inv) :
    GuiElement(game, sf::FloatRect(.2f, .2f, .6f, .6f), sf::Vector2f(110.f, 74.f), GuiAlign::Center, GuiAlign::Center),
    inventory(inv)
{
    //ctor
}

GuiInventory::~GuiInventory()
{
    //dtor
}

bool GuiInventory::handleEvent(sf::Event& event)
{
    return false;
}

void GuiInventory::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(rectangle);
}

void GuiInventory::init()
{
    texture = &getGame().getResourceManager().getTexture("INVENTORY");
    rectangle.setTexture(texture);
    rectangle.setSize(getSize());
    item_draw.setSize(sf::Vector2f(16.f, 16.f));
}

void GuiInventory::update(float delta_time)
{

}
