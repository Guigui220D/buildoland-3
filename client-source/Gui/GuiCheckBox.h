#pragma once

#include "GuiButton.h"

class GuiCheckBox : public GuiButton
{
    public:
        GuiCheckBox(Game& game, sf::FloatRect zone, GuiAlign horizontal_align, GuiAlign vertical_align);
        virtual ~GuiCheckBox();

        virtual void update(float delta_time) override;

        inline bool isChecked() const { return checked; };

    protected:
        bool checked = false;

    private:
};
