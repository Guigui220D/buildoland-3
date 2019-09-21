#include "GuiTitle.h"

GuiTitle::GuiTitle(Game* game) :
    GuiElement(game, sf::FloatRect(.1f, .2f, .8f, .2f), (1000.f / 137.f), GuiAlign::Center, GuiAlign::Center)
{

}

GuiTitle::~GuiTitle()
{
    //dtor
}

void GuiTitle::init()
{
    title_texture.loadFromFile("Resources/Logos/text_version_2.png");
    title_texture.setSmooth(true);
    title_sprite.setTexture(&title_texture);
    title_sprite.setSize(sf::Vector2f((1000.f / 137.f), 1.f));
}

void GuiTitle::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(title_sprite);
}
