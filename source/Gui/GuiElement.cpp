#include "GuiElement.h"

#include "../Game.h"

GuiElement::GuiElement(Game* game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align) :
    game(game),
    zone(zone),
    ratio(aspect_ratio),
    halign(horizontal_align),
    valign(vertical_align)
{
    calculateView(game->getWindow().getSize());
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

void GuiElement::debugDraw(sf::RenderTarget& target) const
{
    //Create rectangle
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(20000.f, 20000.f));
    rectangle.setPosition(sf::Vector2f(-10000.f, -10000.f));
    rectangle.setFillColor(sf::Color::Green);
    //Make view for zone
    sf::View v;
    v.setSize(sf::Vector2f(1.f, 1.f));
    v.setViewport(zone);
    target.setView(v);
    //Draw green rectangle
    target.draw(rectangle);
    useView(target);
    //Draw red rectangle
    rectangle.setFillColor(sf::Color::Red);
    target.draw(rectangle);
}

void GuiElement::update(float delta_time) {}

void GuiElement::calculateView(sf::Vector2u window_size)
{
    sf::View view(sf::FloatRect(0, 0, ratio, 1.f));
    sf::FloatRect zone_rect(zone.left * window_size.x, zone.top * window_size.y, zone.width * window_size.x, zone.height * window_size.y);
    float zone_ratio = zone_rect.width / zone_rect.height;

    if (zone_ratio < ratio)
    {
        float height = (zone_rect.width / ratio) / window_size.y;
        switch (valign)
        {
        case GuiAlign::Center:
            view.setViewport(sf::FloatRect(zone.left, zone.top + (zone.height - height) / 2.f, zone.width, height));
            break;
        case GuiAlign::BottomOrRight:
            view.setViewport(sf::FloatRect(zone.left, zone.top + (zone.height - height), zone.width, height));
            break;
        case GuiAlign::TopOrLeft:
            view.setViewport(sf::FloatRect(zone.left, zone.top, zone.width, height));
            break;
        }
    }
    else
    {
        float width = (zone_rect.height * ratio) / window_size.x;
        switch (halign)
        {
        case GuiAlign::Center:
            view.setViewport(sf::FloatRect(zone.left + (zone.width - width) / 2.f, zone.top, width, zone.height));
            break;
        case GuiAlign::BottomOrRight:
            view.setViewport(sf::FloatRect(zone.left + (zone.width - width), zone.top, width, zone.height));
            break;
        case GuiAlign::TopOrLeft:
            view.setViewport(sf::FloatRect(zone.left, zone.top, width, zone.height));
            break;
        }
    }

    calculated_view = view;
}
