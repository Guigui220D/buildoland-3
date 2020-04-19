#pragma once

#include "State.h"

#include "../Gui/GuiButton.h"
#include "../Gui/GuiZone.h"
#include "../Gui/TextInput.h"
#include "../Gui/GuiLabel.h"

class NicknameChoiceState : public State
{
public:
    NicknameChoiceState(Game& game, unsigned int id = 0);
    ~NicknameChoiceState();

    void init() override;

    bool handleEvent(sf::Event& event) override;
    void update(float delta_time) override;
    void draw(sf::RenderTarget& target) const override;
    void updateView() override;

private:
    GuiZone gui_zone;

    GuiButton start_button;
    GuiButton main_menu_button;
    GuiLabel  error_label;
    GuiLabel  ip_label;
    TextInput ip_input;
    GuiLabel  nickname_label;
    TextInput nickname_input;
    //GuiElement test_ge_2;
    //GuiElement test_ge_3;

};
