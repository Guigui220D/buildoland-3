#pragma once

#include "GameState.h"
#include "LoadingScreenState.h"

#include "../Gui/GuiButton.h"
#include "../Gui/GuiLabel.h"

class ErrorState : public State
{
    public:
        template <typename... FmtArgs>
        ErrorState(Game& game, unsigned int id, sf::String error, FmtArgs&&... fmt_args) :
            State(game, id),
            go_back_button(game, sf::FloatRect(.1f, .7f, .8f, .1f), sf::Vector2f(8.f, 1.f), GuiAlign::Center, GuiAlign::Center, "BACK_TO_MAIN_MENU_BUTTON"),
              message(game, sf::FloatRect(.05f, .4f, .9f, .2f), sf::Vector2f(8.f, 1.f), GuiAlign::Center, GuiAlign::Center, error, std::forward<FmtArgs>(fmt_args)...)
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
        GuiButton go_back_button;
        GuiLabel message;
};
