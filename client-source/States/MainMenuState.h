#pragma once

#include "State.h"

#include "../Gui/GuiButton.h"
#include "../Gui/GuiZone.h"

class MainMenuState : public State
{
    public:
        MainMenuState(Game& game, unsigned int id = 0);
        ~MainMenuState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

    private:
        GuiZone gui_zone;

        GuiButton* const test_button_1;
        GuiButton* const test_button_2;
        //GuiElement test_ge_2;
        //GuiElement test_ge_3;

};
