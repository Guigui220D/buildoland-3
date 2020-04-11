#include "InventoryMenuState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../../client-source/Settings/BindingsManager.h"

#include "../Gui/GuiInventory.h"
#include "../../common-source/Items/playerInventory.h"

#include "../../client-source/Game.h"

InventoryMenuState::InventoryMenuState(Game& game, PlayerInventory& inv, unsigned int id) :
    State(game, id),
    gui_zone(game, sf::Vector2f(.75f, .55f), GuiZone::Center, GuiZone::Middle),
    inventory(new GuiInventory(game, inv))
{
    update_transparent = true;
    draw_transparent = true;

}

InventoryMenuState::~InventoryMenuState()
{
    //dtor
}

void InventoryMenuState::init()
{
    gui_zone.addElement(inventory);
    gui_zone.init();
}

bool InventoryMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
    {
        gui_zone.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    }
    gui_zone.handleEvent(event);
    return true;
}

void InventoryMenuState::update(float delta_time)
{
    gui_zone.update(delta_time);

    if (!getGame().getBindingsManager().held("inventory"))
        must_be_destroyed = true;
}

void InventoryMenuState::draw(sf::RenderTarget& target) const
{
    //gui_zone.debugDraw(target);
    gui_zone.draw(target);
}

void InventoryMenuState::updateView()
{
    gui_zone.calculateView(getGame().getWindow().getSize());
}
