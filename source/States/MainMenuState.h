#pragma once

#include "State.h"
#include "../Gui/GuiButton.h"

class MainMenuState : public State
{
    public:
        MainMenuState(Game* game, unsigned int id = 0);
        ~MainMenuState();

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;

    private:
        GuiButton test_button_1;
        //GuiElement test_ge_2;
        //GuiElement test_ge_3;

};
