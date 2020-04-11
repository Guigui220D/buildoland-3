#pragma once

#include "GameState.h"
#include "LoadingScreenState.h"

#include "../Gui/GuiZone.h"
#include "../Gui/GuiButton.h"
#include "../Gui/GuiLabel.h"

class ErrorState : public State
{
    public:
        template <typename... FmtArgs>
        ErrorState(Game& game, unsigned int id, sf::String error, FmtArgs&&... fmt_args) :
            State(game, id),
            gui_zone(game, sf::Vector2f(.8f, .5f), GuiZone::Center, GuiZone::Middle),
            go_back_button(new GuiButton(game, sf::Vector2f(0.f, 200.f), 800.f, "BACK_TO_MAIN_MENU_BUTTON")),
            message(new GuiLabel(game, sf::Vector2f(400.f, 50.f), error, std::forward<FmtArgs>(fmt_args)...))
        {
            draw_transparent = false;
        }

        ~ErrorState();

        void init() override;

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;
        void updateView() override;

    private:
        GuiZone gui_zone;
        GuiButton* go_back_button;
        GuiLabel* message;
};
