#include "GuiManager.h"

GuiManager::GuiManager()
{

}

GuiManager::~GuiManager()
{

}

void GuiManager::initEverything()
{
    for (auto i = zones.begin(); i < zones.end(); i++)
        i->init();
}

void GuiManager::updateWindowSize(sf::Vector2u window_size)
{
    for (auto i = zones.begin(); i < zones.end(); i++)
        i->calculateView(window_size);
}

bool GuiManager::handleEvent(sf::Event& event)
{
    for (auto i = zones.begin(); i < zones.end(); i++)
    {
        if (i->handleEvent(event))
            return true;
    }
    return false;
}

void GuiManager::updateEverything(float delta_time)
{
    for (auto i = zones.begin(); i < zones.end(); i++)
        i->update(delta_time);
}

void GuiManager::drawEverything(sf::RenderTarget& target) const
{
    for (auto i = zones.begin(); i < zones.end(); i++)
        i->draw(target);
}
