#include "GuiZone.h"

#include "GuiElement.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../Game.h"

GuiZone::GuiZone(Game& game, sf::Vector2f size_portion, HAlign horizontal_align, VAlign vertical_align) :
    game(game),
    h_align(horizontal_align),
    v_align(vertical_align),
    size_portion(size_portion),
    usable_size(size_portion * 1000.f)
{
    test_rect.setSize(usable_size);
    //test_rect.setOrigin(usable_size / 2.f);
    test_rect.setFillColor(sf::Color(0, 255, 0, 127));

    test_2.setFillColor(sf::Color(255, 0, 0, 127));
    test_2.setRadius(10.f);
    test_2.setOrigin(sf::Vector2f(10.f, 10.f));
}

GuiZone::~GuiZone()
{
    //dtor
}

void GuiZone::calculateView(sf::Vector2u window_size)
{
    sf::View new_view;

    sf::Vector2f view_size(window_size.x, window_size.y);

    view_size *= 1000.f / window_size.y;

    new_view.setSize(view_size);
    new_view.setCenter(sf::Vector2f());

    switch (h_align)
    {
    case Left:
        new_view.move(sf::Vector2f(view_size.x / 2.f, 0));
        break;

    case Center:
        new_view.move(sf::Vector2f(usable_size.x / 2.f, 0));
        break;

    case Right:
        new_view.move(sf::Vector2f(-view_size.x / 2.f + usable_size.x, 0));
        break;
    }

    switch (v_align)
    {
    case Top:
        new_view.move(sf::Vector2f(0, view_size.y / 2.f));
        break;

    case Middle:
        new_view.move(sf::Vector2f(0, usable_size.y / 2.f));
        break;

    case Bottom:
        new_view.move(sf::Vector2f(0, -view_size.y / 2.f + usable_size.y));
        break;
    }

    my_view = new_view;
}

void GuiZone::debugDraw(sf::RenderTarget& target) const
{
    target.setView(my_view);
    target.draw(test_rect);
    target.draw(test_2);
}

void GuiZone::init()
{
    calculateView(game.getWindow().getSize());

    for (GuiElement* ge : elements)
        ge->init();
}

bool GuiZone::handleEvent(sf::Event& event)
{
    game.getWindow().setView(my_view);
    for (GuiElement* ge : elements)
    {
        if (ge->handleEvent(event))
            return true;
    }
    return false;
}

void GuiZone::update(float delta_time)
{
    game.getWindow().setView(my_view);
    for (GuiElement* ge : elements)
        ge->update(delta_time);
}

void GuiZone::draw(sf::RenderTarget& target) const
{
    target.setView(my_view);
    for (GuiElement* ge : elements)
        ge->draw(target);
}

void GuiZone::setTransparency(uint8_t fade)
{
    for (GuiElement* ge : elements)
        ge->setTransparency(fade);
}
