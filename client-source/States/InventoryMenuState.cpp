#include "InventoryMenuState.h"

InventoryMenuState::InventoryMenuState(Game& game, PlayerInventory& inv, unsigned int id) :
    State(game, id),
    inventory(game, inv)
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
    inventory.init();
}

bool InventoryMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
    {
        inventory.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    }
    inventory.handleEvent(event);
    return true;
}

void InventoryMenuState::update(float delta_time)
{
    inventory.update(delta_time);

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        must_be_destroyed = true;
}

void InventoryMenuState::draw(sf::RenderTarget& target) const
{
    inventory.draw(target);
}

void InventoryMenuState::updateView()
{

}
