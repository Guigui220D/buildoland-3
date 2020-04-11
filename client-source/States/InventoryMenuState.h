#pragma once

#include "State.h"

#include "../Gui/GuiZone.h"

class PlayerInventory;
class GuiInventory;

class InventoryMenuState : public State
{
    public:
        InventoryMenuState(Game& game, PlayerInventory& inv, unsigned int id = 0);
        ~InventoryMenuState();

        void init() override;
        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

    private:
        GuiZone gui_zone;
        GuiInventory* inventory;
};
