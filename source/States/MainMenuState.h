#pragma once

#include "State.h"
#include "../Gui/GuiElement.h"

class MainMenuState : public State
{
    public:
        MainMenuState(Game* game, unsigned int id = 0);
        virtual ~MainMenuState();

        virtual bool handleEvent(sf::Event& event) override;
        virtual void draw(sf::RenderTarget& target) const override;

    private:
        GuiElement test_ge_1;
        //GuiElement test_ge_2;
        //GuiElement test_ge_3;

};
