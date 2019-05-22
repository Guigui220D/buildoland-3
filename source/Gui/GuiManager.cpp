#include "GuiManager.h"

GuiManager::GuiManager()
{
    //ctor
}

GuiManager::~GuiManager()
{
    for (auto i = begin(); i < end(); i++)
        delete *i;
}

void GuiManager::initEverything()
{
    /* INIT HERE
    for (auto i = begin(); i < end(); i++)
        (*i)->
        */
}

void GuiManager::updateWindowSize(sf::Vector2u window_size)
{
    for (auto i = begin(); i < end(); i++)
        (*i)->calculateView(window_size);
}

bool GuiManager::handleEvent(sf::Event& event)
{
    for (auto i = begin(); i < end(); i++)
    {
        if ((*i)->handleEvent(event))
            return true;
    }
    return false;
}

void GuiManager::updateEverything(float delta_time)
{
    for (auto i = begin(); i < end(); i++)
        (*i)->update(delta_time);
}

void GuiManager::drawEverything(sf::RenderTarget& target)
{
    for (auto i = begin(); i < end(); i++)
        (*i)->draw(target);
}
