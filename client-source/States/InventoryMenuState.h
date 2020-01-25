#pragma once

#include "State.h"

#include "../Gui/GuiInventory.h"

#include "../../common-source/Items/PlayerInventory.h"

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
        GuiInventory inventory;
};
