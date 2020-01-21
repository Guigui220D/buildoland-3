#pragma once

#include "GuiElement.h"

class GuiLabel : public GuiElement
{
    public:
        GuiLabel(Game& game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align, const std::string message);
        ~GuiLabel();

        void draw(sf::RenderTarget& target) const override;
        void init() override;

    private:
        sf::Text text;
        const std::string message;
};
