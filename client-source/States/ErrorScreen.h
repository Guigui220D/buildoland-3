#pragma once

#include "GameState.h"
#include "LoadingScreenState.h"

#include "../Gui/GuiButton.h"
#include "../Gui/GuiLabel.h"

class ErrorState : public State
{
    public:
        ErrorState(Game& game, sf::String error, unsigned int id = 0);
        ~ErrorState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

    private:
        GuiButton go_back_button;
        GuiLabel message;
};
