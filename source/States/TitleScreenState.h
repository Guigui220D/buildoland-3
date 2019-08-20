#pragma once

#include "LoadingScreenState.h"
#include "MainMenuState.h"

#include "../Gui/GuiTitle.h"

class TitleScreenState : public LoadingScreenState<MainMenuState>
{
    public:
        TitleScreenState(Game* game, unsigned int id);
        ~TitleScreenState();

        void init() override;
        bool handleEvent(sf::Event& event) override;

    private:
        void drawMore(sf::RenderTarget& target) const override;
        void updateMore(float delta_time) override;

        void beforeInitTask() override;
        void afterInitTask() override;

        sf::Clock clk;

        GuiTitle title;
};
