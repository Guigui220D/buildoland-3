#pragma once

#include "GameState.h"
#include "LoadingScreenState.h"

#include "../Gui/GuiManager.h"
#include "../Gui/GuiButton.h"
#include "../Gui/GuiCheckBox.h"

class MainMenuState : public State
{
    public:
        MainMenuState(Game* game, unsigned int id = 0);
        ~MainMenuState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

    private:
        GuiManager gui_manager;

        GuiButton* const test_button_1;
        GuiButton* const test_button_2;
        GuiCheckBox* const serv_debug_checkbox;
        //GuiElement test_ge_2;
        //GuiElement test_ge_3;

};
